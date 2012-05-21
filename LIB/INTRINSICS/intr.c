#include <stdio.h>
#include <pmmintrin.h>

int main() {
  double a[2],b[2],c[2],d[2],e[2];
  a[0]=3;
  a[1]=4.5;
  b[0]=3.3;
  b[1]=44.1;
  c[0]=5;
  c[1]=7.4;
  d[0]=7.1;
  d[1]=24.4;
  __m128d sum1,sum2;
  const __m128d *aptr=(const __m128d *)&a[0];
  const __m128d *bptr=(const __m128d *)&b[0];
  const __m128d *cptr=(const __m128d *)&c[0];
  const __m128d *dptr=(const __m128d *)&d[0];
  __m128d *eptr=(__m128d *)&e[0];
  /*  sum1 = [3*3.3 4.5*44.1] */
  sum1=_mm_mul_pd(*aptr,*bptr);
  printf("%f %f\n",*(double *)(&sum1),*((double *)(&sum1)+1));
  /*  sum2 = [5*7.1 7.4*24.4] */
  sum2=_mm_mul_pd(*cptr,*dptr);
  printf("%f %f\n",*(double *)(&sum2),*((double *)(&sum2)+1));
  *eptr = _mm_hadd_pd(sum1,sum2);
  printf("%f %f\n",e[0],e[1]);
  return 0;
}
