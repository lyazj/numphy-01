#include "defs.h"
#include <math.h>
#include <stdio.h>

static void pivot(int n, int m, number A[n][m], int i);

int gem(int n, int m, number A[n][m])
{
  int i, j, k;
  number Aii, s;

  for(i = 0; i < n - 1; ++i)
  {
    pivot(n, m, A, i);
    Aii = A[i][i];
    if(Aii == 0)
    {
      fprintf(stderr, "%s: error: A[%d][%d] == 0\n", __func__, i, i);
      return i;
    }
    for(k = i + 1; k < n; ++k)
    {
      s = -A[k][i] / Aii;
      for(j = i + 1; j < m; ++j)
        A[k][j] += s * A[i][j];
    }
  }
  if(A[i][i] == 0)
    return i;
  for(i = n - 1; i >= 0; --i)
  {
    for(k = i + 1; k < n; ++k)
      for(j = n; j < m; ++j)
        A[i][j] -= A[i][k] * A[k][j];
    Aii = A[i][i];
    for(j = n; j < m; ++j)
      A[i][j] /= Aii;
  }
  return n;
}

void pivot(int n, int m, number A[n][m], int i)
{
  number p = fabs(A[i][i]), pk;
  int r = i, k;

  for(k = i + 1; k < n; ++k)
    if((pk = fabs(A[k][i])) > p)
    {
      p = pk;
      r = k;
    }
  if(r != i)
  {
    for(k = i; k < m; ++k)
    {
      p = A[i][k];
      A[i][k] = A[r][k];
      A[r][k] = p;
    }
  }
}
