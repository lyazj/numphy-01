#include "defs.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

static void cdm_test_1(void);
static void cdm_test_2(void);

int main(void)
{
  cdm_test_1();
  cdm_test_2();
  return 0;
}

void cdm_test_1(void)
{
  number A[2][3] = {
    {2, 1, 3},
    {1, 2, 0},
  };
  int i;

  cdm(2, 3, A);
  for(i = 0; i < 2; ++i)
    printf("%g\n", (double)A[i][2]);
  assert(fabs(A[0][2] -  2) < 1e-15);
  assert(fabs(A[1][2] - -1) < 1e-15);
}

void cdm_test_2(void)
{
  number A[2][4] = {
    {2, 1, 1, 0},
    {1, 2, 0, 1},
  };
  number X[2][2] = {
    { 2.0/3.0, -1.0/3.0},
    {-1.0/3.0,  2.0/3.0},
  };
  int i, j;

  cdm(2, 4, A);
  for(i = 0; i < 2; ++i)
  {
    for(j = 2; j < 4; ++j)
      printf("%16g", A[i][j]);
    printf("\n");
  }
  for(i = 0; i < 2; ++i)
    for(j = 2; j < 4; ++j)
      assert(fabs(A[i][j] - X[i][j - 2]) < 1e-15);
}
