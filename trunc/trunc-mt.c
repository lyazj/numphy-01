#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif  /* M_PI */

#define number double
#define Q2 0.5
#define Q (sqrt(Q2))
#define L 1000000  /* this is OK as we have SIGINT/SIGTERM */
#define NTHREAD 64

static void dS(number df[L + 1], int l, number q2);
// static void dI(number df[L + 1], int l, number q);

static void d3S(number df[L + 1], int i, int j, int k, number q2, number w);

typedef struct {
  pthread_mutex_t lock;
  int value;
} l_t, *l_p;

typedef struct {
  number df[L + 1];
  sem_t *sem;
  l_p lp;
  number q2;
} job_t, *job_p;

typedef struct {
  l_t l;
  sem_t sem[L + 1];
  job_t job[NTHREAD];
  pthread_t thr[NTHREAD];
} ctl_t, *ctl_p;

static void *job_proc(void *);
static number dS_mt(ctl_p ctlp, int l);
static number dI_mt(int l, number q);

static void l_init(l_p lp)
{
  pthread_mutex_init(&lp->lock, NULL);
  lp->value = 1;
}

static void *job_proc(void *argp)
{
  job_p jobp = argp;
  number *df = jobp->df;
  sem_t *sem = jobp->sem;
  l_p lp = jobp->lp;
  number q2 = jobp->q2;
  int i = 1, l, r;

  while(i <= L)
  {
    if((r = pthread_mutex_lock(&lp->lock)))
    {
      errno = r;
      err(EXIT_FAILURE, "pthread_mutex_lock");
    }
    l = lp->value++;
    if((r = pthread_mutex_unlock(&lp->lock)))
    {
      errno = r;
      err(EXIT_FAILURE, "pthread_mutex_unlock");
    }
    while(i < l)
    {
      if(sem_post(&sem[i++]))
        err(EXIT_FAILURE, "sem_post");
    }
    dS(df, l, q2);
    if(sem_post(&sem[i++]))
      err(EXIT_FAILURE, "sem_post");
  }
  return NULL;
}

static void ctl_init(ctl_p ctlp, number q2)
{
  int i, j, r;

  l_init(&ctlp->l);
  for(i = 0; i <= L; ++i)
    if(sem_init(&ctlp->sem[i], 0, 0))
      err(EXIT_FAILURE, "sem_init");
  for(i = 0; i < NTHREAD; ++i)
  {
    for(j = 0; j <= L; ++j)
      ctlp->job[i].df[j] = 0;
    ctlp->job[i].sem = ctlp->sem;
    ctlp->job[i].lp = &ctlp->l;
    ctlp->job[i].q2 = q2;
    if((r = pthread_create(&ctlp->thr[i], NULL, job_proc, &ctlp->job[i])))
    {
      errno = r;
      err(EXIT_FAILURE, "pthread_create");
    }
  }
}

static volatile int *contp;
static void signal_handler(int signum)
{
  char msg[64], *p = msg;
  int n, r;
  *contp = 0;
  n = snprintf(msg, sizeof msg,
      "terminating on signal %d...\n", signum);
  while((r = write(STDERR_FILENO, p, n)) != n)
  {
    if(r < 0 && errno != EINTR)
      break;  /* this is an error but I've no idea */
    p += r;
    n -= r;
  }
}
static void signal_handler_init(volatile int *cp)
{
  contp = cp;
  if(signal(SIGINT, signal_handler) == SIG_ERR)
    err(EXIT_FAILURE, "signal");
  if(signal(SIGTERM, signal_handler) == SIG_ERR)
    err(EXIT_FAILURE, "signal");
}

int main(void)
{
  int l;
  volatile int cont = 1;
  static ctl_t ctl;
  number df;
  number f;

  setvbuf(stdout, NULL, _IOLBF, 0);
  signal_handler_init(&cont);
  ctl_init(&ctl, Q2);
  f = df = 1 / -Q2;
  printf("df(%d) = %g\n", 0, df);
  printf("f(%d) = %g\n", 0, f);
  for(l = 1; cont && l <= L; ++l)
  {
    df = dS_mt(&ctl, l) + dI_mt(l, Q);
    f += df;
    printf("df(%d) = %g\n", l, df);
    printf("f(%d) = %g\n", l, f);
  }
  return 0;
}

void d3S(number df[L + 1], int i, int j, int k, number q2, number w)
{
  number l2 = i*i + j*j + k*k;
  int l = ceil(sqrt(l2));

  if(l <= L)
    df[l] += w / (l2 - q2);
}

// requirement: l != 0
void dS(number df[L + 1], int l, number q2)
{
  int i, j;

  // [3] 0: 2, l: 1
  // d3S_2(df, l, 0, 0, q2);
  // d3S_2(df, 0, l, 0, q2);
  // d3S_2(df, 0, 0, l, q2);
  d3S(df, l, 0, 0, q2, 6);

  // [6*(l-1)] 0: 1, l: 1
  for(i = 1; i < l; ++i)
  {
    // d3S_1(df, l, 0, i, q2);
    // d3S_1(df, l, i, 0, q2);
    // d3S_1(df, 0, l, i, q2);
    // d3S_1(df, i, l, 0, q2);
    // d3S_1(df, 0, i, l, q2);
    // d3S_1(df, i, 0, l, q2);
    d3S(df, l, 0, i, q2, 24);
  }

  // [3] 0: 1, l: 2
  // d3S_1(df, 0, l, l, q2);
  // d3S_1(df, l, 0, l, q2);
  // d3S_1(df, l, l, 0, q2);
  d3S(df, l, 0, l, q2, 12);

  // [3*(l-1)*(l-1)] 0: 0, l: 1
  // for(i = 1; i < l; ++i)
  //   for(j = 1; j < l; ++j)
  //   {
  //     d3S_0(df, l, i, j, q2);
  //     d3S_0(df, i, l, j, q2);
  //     d3S_0(df, i, j, l, q2);
  //   }
  for(i = 1; i < l; ++i)
  {
    for(j = 1; j < i; ++j)
      d3S(df, l, i, j, q2, 48);
    d3S(df, l, i, i, q2, 24);
  }

  // [3*(l-1)] 0: 0, l: 2
  for(i = 1; i < l; ++i)
  {
    // d3S_0(df, i, l, l, q2);
    // d3S_0(df, l, i, l, q2);
    // d3S_0(df, l, l, i, q2);
    d3S(df, l, l, i, q2, 24);
  }

  // [1] 0: 0, l: 3
  // d3S_0(df, l, l, l, q2);
  d3S(df, l, l, l, q2, 8);
}

// void dI(number df[L + 1], int l, number q)
// {
//   df[l] -= 4 * M_PI * (1 + q / 2 * log(fabs(
//       ((l-q) / (l+q)) * ((l+q - 1) / (l-q - 1)))
//   ));
// }

number dI_mt(int l, number q)
{
  return -4 * M_PI * (1 + q / 2 * log(fabs(
      ((l-q) / (l+q)) * ((l+q - 1) / (l-q - 1)))
  ));
}

number dS_mt(ctl_p ctlp, int l)
{
  int i;
  number d = 0;

  for(i = 0; i < NTHREAD; ++i)
    while(sem_wait(&ctlp->sem[l]))
      if(errno != EINTR)
        err(EXIT_FAILURE, "sem_wait");
  for(i = 0; i < NTHREAD; ++i)
    d += ctlp->job[i].df[l];
  return d;
}
