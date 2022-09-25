#include "defs.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>

static void cholesky_test_1(void);
static void cholesky_test_2(void);

int main(void)
{
  cholesky_test_1();
  cholesky_test_2();
  return 0;
}

void cholesky_test_1(void)
{
  number A[2][2] = {
    {2, 1},
    {1, 2},
  };
  number C[2][2] = {
    {sqrt(2.0), sqrt(0.5)},
    {sqrt(0.5), sqrt(1.5)},
  };
  int i, j;

  cholesky(2, 2, A);
  for(i = 0; i < 2; ++i)
  {
    for(j = 0; j < 2; ++j)
      printf("%16g", A[i][j]);
    printf("\n");
  }
  for(i = 0; i < 2; ++i)
    for(j = 0; j < 2; ++j)
      assert(fabs(A[i][j] - C[i][j]) < 1e-15);
}

void cholesky_test_2(void)
{
  number A[2][3] = {
    {2, 1, 10},
    {1, 2, 11},
  };
  number C[2][3] = {
    {sqrt(2.0), sqrt(0.5), 10},
    {sqrt(0.5), sqrt(1.5), 11},
  };
  int i, j;

  cholesky(2, 3, A);
  for(i = 0; i < 2; ++i)
  {
    for(j = 0; j < 3; ++j)
      printf("%16g", A[i][j]);
    printf("\n");
  }
  for(i = 0; i < 2; ++i)
    for(j = 0; j < 3; ++j)
      assert(fabs(A[i][j] - C[i][j]) < 1e-15);
}
