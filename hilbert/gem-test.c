#include "defs.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

static void gem_test_1(void);
static void gem_test_2(void);

int main(void)
{
  gem_test_1();
  gem_test_2();
  return 0;
}

void gem_test_1(void)
{
  number A[2][3] = {
    {1, 2, 0},
    {3, 1, 5},
  };
  int i;

  gem(2, 3, A);
  for(i = 0; i < 2; ++i)
    printf("%g\n", (double)A[i][2]);
  assert(fabs(A[0][2] -  2) < 1e-15);
  assert(fabs(A[1][2] - -1) < 1e-15);
}

void gem_test_2(void)
{
  number A[4][8] = {
    { 1, -1, -1, -1, 1, 0, 0, 0},
    { 0,  1, -1, -1, 0, 1, 0, 0},
    { 0,  0,  1, -1, 0, 0, 1, 0},
    { 0,  0,  0,  1, 0, 0, 0, 1},
  };
  number X[4][4] = {
    {1, 1, 2, 4},
    {0, 1, 1, 2},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
  };
  int i, j;

  gem(4, 8, A);
  for(i = 0; i < 4; ++i)
  {
    for(j = 4; j < 8; ++j)
      printf("%8g", A[i][j]);
    printf("\n");
  }
  for(i = 0; i < 4; ++i)
    for(j = 4; j < 8; ++j)
      assert(fabs(A[i][j] - X[i][j - 4]) < 1e-15);
}
