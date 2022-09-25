#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

#define GEM_MAX 200
#define CHO_MAX 200

static void hx1(int n, solve_t solve, const char *name);
static void matdot(int nr, int nc, int n,
    number A[nr][n], number B[n][nc], number AB[nr][nc]);

int main(void)
{
  int i;

  for(i = 1; i <= GEM_MAX; ++i)
  {
    hx1(i, gem, "gem");
    hx1(i, cdm, "cdm");
  }
  return 0;
}

void hx1(int n, solve_t solve, const char *name)
{
  double (*A)[n + 1] = malloc(n * sizeof *A);
  double (*A0)[n] = malloc(n * sizeof *A0);
  double x[n], Ax[n], res, mse = 0;
  int i, j, r;

  for(i = 0; i < n; ++i)
  {
    for(j = 0; j < n; ++j)
      A[i][j] = (number)1 / (i + j + 1);
    A[i][j] = 1;
  }
  for(i = 0; i < n; ++i)
    for(j = 0; j < n; ++j)
      A0[i][j] = A[i][j];
  r = solve(n, n + 1, A);
  if(r != n)
  {
    fprintf(stderr, "%s(%d): failed\n", name, n);
    free(A0);
    free(A);
    return;
  }
  for(i = 0; i < n; ++i)
    x[i] = A[i][n];
  for(i = 0; i < n - 1; ++i)
    printf("%g ", x[i]);
  printf("%g\n", x[i]);
  matdot(n, 1, n, A0, (void *)x, (void *)Ax);
  for(i = 0; i < n; ++i)
  {
    res = 1 - Ax[i];
    mse += res * res;
  }
  mse /= n;
  fprintf(stderr, "%s(%d): mse = %g\n", name, n, (double)mse);
  free(A0);
  free(A);
}

void matdot(int nr, int nc, int n,
    number A[nr][n], number B[n][nc], number AB[nr][nc])
{
  int i, j, k;
  number Aik;

  for(i = 0; i < nr; ++i)
    for(j = 0; j < nc; ++j)
      AB[i][j] = 0;
  for(i = 0; i < nr; ++i)
    for(k = 0; k < n; ++k)
    {
      Aik = A[i][k];
      for(j = 0; j < nc; ++j)
        AB[i][j] += Aik * B[k][j];
    }
}
