#include "defs.h"
#include <math.h>
#include <stdio.h>

int cholesky(int n, int m, number A[n][m])
{
  int i, j, k;
  number Ajj, Ajk, Aij;

  for(j = 0; j < n; ++j)
  {
    Ajj = A[j][j];
    for(k = 0; k < j; ++k)
    {
      Ajk = A[j][k];
      Ajj -= Ajk * Ajk;
    }
    if(Ajj <= 0)
    {
      fprintf(stderr, "%s: error: A[%d][%d] == 0\n", __func__, j, j);
      return j;
    }
    Ajj = sqrt(Ajj);
    A[j][j] = Ajj;
    for(i = j + 1; i < n; ++i)
    {
      Aij = A[i][j];
      for(k = 0; k < j; ++k)
        Aij -= A[i][k] * A[j][k];
      A[j][i] = A[i][j] = Aij / Ajj;
    }
  }
  return n;
}

int cdm(int n, int m, number A[n][m])
{
  int i, j, k, r;

  r = cholesky(n, m, A);
  if(r != n)
    return r;
  for(i = 0; i < n; ++i)
  {
    for(k = 0; k < i; ++k)
      for(j = n; j < m; ++j)
        A[i][j] -= A[i][k] * A[k][j];
    for(j = n; j < m; ++j)
      A[i][j] /= A[i][i];
  }
  for(i = n - 1; i >= 0; --i)
  {
    for(k = i + 1; k < n; ++k)
      for(j = n; j < m; ++j)
        A[i][j] -= A[i][k] * A[k][j];
    for(j = n; j < m; ++j)
      A[i][j] /= A[i][i];
  }
  return r;
}
