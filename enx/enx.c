#include <math.h>
#include <stdio.h>

#if 0  /* alternative */
#define number float
#endif  /* 0 */
#define number double

#define ID2STR(M) #M
#define M2STR(M) ID2STR(M)
#define NUMBER M2STR(number)

number enx_dir(number x)
{
  number k = 0, xk = 1, kf = 1, Sk = 1, sign = 1, ak;

  while(1)
  {
    k += 1;
    xk *= x;
    if(isinf(xk))
    {
      fprintf(stderr, "%s(%g): %g^%g overflow\n", __func__, (double)x, (double)x, (double)k);
      break;
    }
    kf *= k;
    if(isinf(kf))
    {
      fprintf(stderr, "%s(%g): %g! overflow\n", __func__, (double)x, (double)k);
      break;
    }
    sign = -sign;
    ak = sign * xk / kf;
    if(ak == 0)
    {
      fprintf(stderr, "%s(%g): a[%g] underflow\n", __func__, (double)x, (double)k);
      break;
    }
    if(isinf(ak))
    {
      fprintf(stderr, "%s(%g): a[%g] overflow\n", __func__, (double)x, (double)k);
      break;
    }
    if(Sk + ak == Sk)
    {
      fprintf(stderr, "%s(%g): Sk[%g] = %g convergent,"
          " with ak[%g] = %g\n", __func__, (double)x, (double)(k - 1), (double)Sk, (double)k, (double)ak);
      break;
    }
    Sk = Sk + ak;
    if(isinf(Sk))
    {
      fprintf(stderr, "%s(%g): Sk[%g] overflow\n", __func__, (double)x, (double)k);
      break;
    }
  }
  return Sk;
}

number enx_rec(number x)
{
  number k = 0, ak = 1, Sk = 1;

  while(1)
  {
    k += 1;
    ak *= -1 * x / k;
    if(ak == 0)
    {
      fprintf(stderr, "%s(%g): a[%g] underflow\n", __func__, (double)x, (double)k);
      break;
    }
    if(isinf(ak))
    {
      fprintf(stderr, "%s(%g): a[%g] overflow\n", __func__, (double)x, (double)k);
      break;
    }
    if(Sk + ak == Sk)
    {
      fprintf(stderr, "%s(%g): Sk[%g] = %g convergent,"
          " with ak[%g] = %g\n", __func__, (double)x, (double)(k - 1), (double)Sk, (double)k, (double)ak);
      break;
    }
    Sk = Sk + ak;
    if(isinf(Sk))
    {
      fprintf(stderr, "%s(%g): Sk[%g] overflow\n", __func__, (double)x, (double)k);
      break;
    }
  }
  return Sk;
}

int main(int argc, char *argv[])
{
  number x, std, dir, rec, inv;

  if(argc != 1)
  {
    fprintf(stderr, "%s: expect no argument, got %d\n", argv[0], argc);
    return 1;
  }
  printf("number = " NUMBER);
  for(x = 0; x <= 100; x += 10)
  {
    printf("\n");
    std = exp(-x);
    dir = enx_dir(x);
    rec = enx_rec(x);
    inv = 1 / enx_rec(-x);
    printf("std: exp(-%g) = %g (%+g)\n", (double)x, (double)std, (double)(std - std));
    printf("dir: exp(-%g) = %g (%+g)\n", (double)x, (double)dir, (double)(dir - std));
    printf("rec: exp(-%g) = %g (%+g)\n", (double)x, (double)rec, (double)(rec - std));
    printf("inv: exp(-%g) = %g (%+g)\n", (double)x, (double)inv, (double)(inv - std));
  }
  return 0;
}
