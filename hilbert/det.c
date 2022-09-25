#include <math.h>
#include <stdio.h>

#define number double
#define NMAX 10

// 计算 \log c_1 至 \log c_{2n}
void logc(int n, number lc_out[2 * n + 1])
{
  number lf = 0;  // \log k!
  number lc = 0;  // \log c_n = \log 1! + ... + \log (n - 1)!
  int i;

  lc_out[0] = 0.0 / 0.0;
  lc_out[1] = 0;
  for(i = 2; i < 2 * n; ++i)
  {
    lc += lf;
    lf += log(i);
    lc_out[i] = lc;
  }
  lc_out[i] = lc + lf;
}

int main(void)
{
  number lc[2 * NMAX + 1];
  int i;

  logc(NMAX, lc);
  for(i = 1; i <= NMAX; ++i)
    printf("%d\t %g\n", i, exp(4 * lc[i] - lc[2 * i]));
  return 0;
}
