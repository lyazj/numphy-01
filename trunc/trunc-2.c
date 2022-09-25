#ifndef __cplusplus
#error "expect a C++ compiler"
#endif

#include <stdio.h>
#include <math.h>

#define number double
#define Q (sqrt(0.5))
#define L (sqrt(M_PI * 1e5))

static number f(number l, number q);
static number I(number l, number q);
static number S(number l, number q);

int main(void)
{
  number fQ;

  printf("q = %g\n", (double)Q);
  printf("\\Lambda = %g\n", (double)L);
  fQ = f(2 * L, Q);
  printf("f_{2\\Lambda}(q^2) = %g\n", (double)fQ);
  return 0;
}

number f(number l, number q)
{
  return S(l, q) - I(l, q);
}

number I(number l, number q)
{
  return 4 * M_PI * (l + q / 2 * log(fabs((l - q) / (l + q))));
}

number S(number l, number q)
{
  number s = 0, l2 = l * l, q2 = q * q;
  int i, j, k, n2, lc = ceil(l);

  for(i = -lc; i <= lc; ++i)
    for(j = -lc; j <= lc; ++j)
      for(k = -lc; k <= lc; ++k)
        if((n2 = i*i + j*j + k*k) <= l2)
          s += 1 / (n2 - q2);
  return s;
}
