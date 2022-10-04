#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif  /* M_PI */

#define number double
#define Q2 0.5
#define Q (sqrt(Q2))
#define L 1000000  /* this is OK as we have SIGINT/SIGTERM */

static void dS(number df[L + 1], int l, number q2);
static void dI(number df[L + 1], int l, number q);

static void d3S(number df[L + 1], int i, int j, int k, number q2, number w);

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
  static number df[L + 1];
  number f;

  signal_handler_init(&cont);
  f = df[0] = 1 / -Q2;
  printf("df(%d) = %g\n", 0, df[0]);
  printf("f(%d) = %g\n", 0, f);
  for(l = 1; cont && l <= L; ++l)
  {
    dS(df, l, Q2);
    dI(df, l, Q);
    f += df[l];
    printf("df(%d) = %g\n", l, df[l]);
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

void dI(number df[L + 1], int l, number q)
{
  df[l] -= 4 * M_PI * (1 + q / 2 * log(fabs(
      ((l-q) / (l+q)) * ((l+q - 1) / (l-q - 1)))
  ));
}
