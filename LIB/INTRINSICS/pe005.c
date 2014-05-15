/* rather silly solution to project euler 5 using intrinsics.
   this program requires AVX2 (haswell and newer) and will die horribly if run
   on a non-conforming cpu. */

#include <stdio.h>
#include <sys/time.h>
#include <immintrin.h>

double gettime() {
	struct timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec+t.tv_usec/1000000.;
}

/* stupid algorithm, try each integer and check if it's divisible by all numbers
   from 1 to 20 */
int pe005() {
	int i,j;
	for(i=1;;i++) {
		for(j=1;j<21;j++) if(i%j) goto fail;
		return i;
	fail:;
	}
}

/* keep a vector of 20 elements, one for each divisor */
/* check all moduli simultaneously */
int pe005intr() {
	__m256i comp=_mm256_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
	__m256i mod=_mm256_set1_epi8(1);
	__m256i add=mod,res,zero=_mm256_setzero_si256();
	int i;
	for(i=1;;i++) {
		/* if all elements in mod are zero, then terminate and return i */
		res=_mm256_cmpeq_epi8(mod,zero);
		/* check top bit from each element */
		if(_mm256_movemask_epi8(res)==-1) return i;
		/* increment */
		mod=_mm256_add_epi8(mod,add);
		/* get result */
		res=_mm256_cmpgt_epi8(mod,comp);
		/* blend: set to 0 if true, otherwise keep value */
		mod=_mm256_blendv_epi8(mod,zero,res);
	}
}

int main() {
	double tid;
	int ans;
	tid=-gettime(); ans=pe005intr(); tid+=gettime();
	printf("intrinsics: %d (%.3f s)\n",ans,tid);
	tid=-gettime(); ans=pe005(); tid+=gettime();
	printf("naive:      %d (%.3f s)\n",ans,tid);
	return 0;
}
