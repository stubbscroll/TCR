/* rather silly solution to project euler 1 using intrinsics.
   this program requires AVX2 (haswell and newer) and will die horribly if run
   on a non-conforming cpu.
   no effort was made to get this fast or anything, only to get it working with
   appropriate instructions */

#include <stdio.h>
#include <sys/time.h>
#include <immintrin.h>

#ifdef __MINGW64__
	#define LONG "%I64d"
#elif defined (_MSC_VER)
	#define LONG "%I64d"
#else
	#define LONG "%lld"
#endif

typedef long long ll;
#define N 1000000000

double gettime() {
	struct timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec+t.tv_usec/1000000.;
}

/* naive C solution using counters modulo 3 and 5 to make it more similar to
   the SSE solution */
ll pe001() {
	ll ans=0;
	int i,tre=1,fem=1;
	for(i=1;i<=N;i++) {
		if(!tre || !fem) ans+=i;
		tre++; if(tre==3) tre=0;
		fem++; if(fem==5) fem=0;
	}
	return ans;
}

/* process 8 elements in each loop iteration.
   no if instruction in loop (except the loop condition).
   we're lazy and require N to be a multiple of 8 */
ll pe001intr1() {
	ll ans=0;
	int _val[8]={1,2,3,4,5,6,7,8};
	int _all8[8]={8,8,8,8,8,8,8,8};
	int _mod3[8]={1,2,0,1,2,0,1,2};
	int _mod5[8]={1,2,3,4,0,1,2,3};
	int i;
	__m256i val,all8,mod3,mod5,zero,res3,res5,all2,all3,all4,all5,pix;
	/* load arrays into 256-bit variables */
	val=_mm256_loadu_si256((__m256i const *)_val);
	all8=_mm256_loadu_si256((__m256i const *)_all8);
	mod3=_mm256_loadu_si256((__m256i const *)_mod3);
	mod5=_mm256_loadu_si256((__m256i const *)_mod5);
	/* we can also set variables directly */
	all2=_mm256_set_epi32(2,2,2,2,2,2,2,2);
	/* we can also set all elements to the same value */
	all3=_mm256_set1_epi32(3);
	all4=_mm256_set1_epi32(4);
	all5=_mm256_set1_epi32(5);
	/* we can also set to zero */
	zero=_mm256_setzero_si256();
	/* permutation indexes */
	pix=_mm256_setr_epi32(0,2,1,3,4,6,5,7);
	for(i=0;i<N;i+=8) {
		/* compare numbers modulo 3 and 5 to 0 and set result vector to true (-1)
		   or false (0) */
		res3=_mm256_cmpeq_epi32(mod3,zero);
		res5=_mm256_cmpeq_epi32(mod5,zero);
		/* or result vectors together. now a vector element is true (-1) if it is
		   divisible by either 3 or 5 */
		res3=_mm256_or_si256(res3,res5);
		/* if element i in res3 is true, then the i-th element is taken from val,
		   otherwise it's taken from zero. unfortunately, even though blendv is
		   content-oblivious, it expects float so we have to cast. in assembly
		   this would look nicer. */
		res3=(__m256i)_mm256_blendv_ps((__m256)zero,(__m256)val,(__m256)res3);
		/* put partial sums in 0,1,4,5 and 0 elsewhere */
		res3=_mm256_hadd_epi32(res3,zero);
		/* convert to 64-bits, results in 0,1,2,3 */
		res3=_mm256_permutevar8x32_epi32(res3,pix);
		/* actually, it's faster to add the 4 values we have here than to proceed
		   with shifting, but where's the fun in that? */
		/* add shifted version, results in 0,1 */
		res3=_mm256_add_epi64(res3,_mm256_srli_si256(res3,8));
		/* only two values left, add them */
		ans+=((ll *)&res3)[0];
		ans+=((ll *)&res3)[2];

		/* increase val by 8 */
		val=_mm256_add_epi32(val,all8);

		/* increase mod3 by 2 (which is 8 modulo 3) */
		mod3=_mm256_add_epi32(mod3,all2);
		/* elements in res3 are -1 (true) for elements in mod3 > 2 */
		res3=_mm256_cmpgt_epi32(mod3,all2);
		/* mod3 - 3 for all elements */
		res5=_mm256_sub_epi32(mod3,all3);
		/* mod3 - 3 only where res3=true, ugly casting again */
		mod3=(__m256i)_mm256_blendv_ps((__m256)mod3,(__m256)res5,(__m256)res3);

		/* increase mod5 by 3 (which is 8 modulo 5) */
		mod5=_mm256_add_epi32(mod5,all3);
		/* elements in res3 are -1 (true) for elements in mod5 > 4 */
		res3=_mm256_cmpgt_epi32(mod5,all4);
		/* mod5 - 5 for all elements */
		res5=_mm256_sub_epi32(mod5,all5);
		/* mod5 - 5 only where res3=true, ugly casting again */
		mod5=(__m256i)_mm256_blendv_ps((__m256)mod5,(__m256)res5,(__m256)res3);
	}
	return ans;
}

/* slightly faster version, doing the sum in a faster (non-sse) way */
ll pe001intr2() {
	ll ans=0;
	int _val[8]={1,2,3,4,5,6,7,8};
	int _all8[8]={8,8,8,8,8,8,8,8};
	int _mod3[8]={1,2,0,1,2,0,1,2};
	int _mod5[8]={1,2,3,4,0,1,2,3};
	int i;
	__m256i val,all8,mod3,mod5,zero,res3,res5,all2,all3,all4,all5;
	/* load arrays into 256-bit variables */
	val=_mm256_loadu_si256((__m256i const *)_val);
	all8=_mm256_loadu_si256((__m256i const *)_all8);
	mod3=_mm256_loadu_si256((__m256i const *)_mod3);
	mod5=_mm256_loadu_si256((__m256i const *)_mod5);
	/* we can also set variables directly */
	all2=_mm256_set_epi32(2,2,2,2,2,2,2,2);
	/* we can also set all elements to the same value */
	all3=_mm256_set1_epi32(3);
	all4=_mm256_set1_epi32(4);
	all5=_mm256_set1_epi32(5);
	/* we can also set to zero */
	zero=_mm256_setzero_si256();
	for(i=0;i<N;i+=8) {
		/* compare numbers modulo 3 and 5 to 0 and set result vector to true (-1)
		   or false (0) */
		res3=_mm256_cmpeq_epi32(mod3,zero);
		res5=_mm256_cmpeq_epi32(mod5,zero);
		/* or result vectors together. now a vector element is true (-1) if it is
		   divisible by either 3 or 5 */
		res3=_mm256_or_si256(res3,res5);
		/* if element i in res3 is true, then the i-th element is taken from val,
		   otherwise it's taken from zero. unfortunately, even though blendv is
		   content-oblivious, it expects float so we have to cast. in assembly
		   this would look nicer. */
		res3=(__m256i)_mm256_blendv_ps((__m256)zero,(__m256)val,(__m256)res3);
		/* put partial sums in 0,1,4,5 and 0 elsewhere */
		res3=_mm256_hadd_epi32(res3,zero);
		/* take the sum now */
		ans+=((int *)&res3)[0];
		ans+=((int *)&res3)[1];
		ans+=((int *)&res3)[2];
		ans+=((int *)&res3)[3];
		ans+=((int *)&res3)[4];
		ans+=((int *)&res3)[5];
		ans+=((int *)&res3)[6];
		ans+=((int *)&res3)[7];

		/* increase val by 8 */
		val=_mm256_add_epi32(val,all8);

		/* increase mod3 by 2 (which is 8 modulo 3) */
		mod3=_mm256_add_epi32(mod3,all2);
		/* elements in res3 are -1 (true) for elements in mod3 > 2 */
		res3=_mm256_cmpgt_epi32(mod3,all2);
		/* mod3 - 3 for all elements */
		res5=_mm256_sub_epi32(mod3,all3);
		/* mod3 - 3 only where res3=true, ugly casting again */
		mod3=(__m256i)_mm256_blendv_ps((__m256)mod3,(__m256)res5,(__m256)res3);

		/* increase mod5 by 3 (which is 8 modulo 5) */
		mod5=_mm256_add_epi32(mod5,all3);
		/* elements in res3 are -1 (true) for elements in mod5 > 4 */
		res3=_mm256_cmpgt_epi32(mod5,all4);
		/* mod5 - 5 for all elements */
		res5=_mm256_sub_epi32(mod5,all5);
		/* mod5 - 5 only where res3=true, ugly casting again */
		mod5=(__m256i)_mm256_blendv_ps((__m256)mod5,(__m256)res5,(__m256)res3);
	}
	return ans;
}

/* keep vector of running sums and sum at the end.
   idea and method stolen from assembly code generated by gcc of similar
   code, but slower than intr2 still (at least on O3 which messes around
   a lot */
ll pe001intr3() {
	int _val[8]={1,2,3,4,5,6,7,8};
	int _all8[8]={8,8,8,8,8,8,8,8};
	int _mod3[8]={1,2,0,1,2,0,1,2};
	int _mod5[8]={1,2,3,4,0,1,2,3};
	int i;
	__m256i val,all8,mod3,mod5,zero,res3,res5,all2,all3,all4,all5,vsum;
	/* load arrays into 256-bit variables */
	val=_mm256_loadu_si256((__m256i const *)_val);
	all8=_mm256_loadu_si256((__m256i const *)_all8);
	mod3=_mm256_loadu_si256((__m256i const *)_mod3);
	mod5=_mm256_loadu_si256((__m256i const *)_mod5);
	/* we can also set variables directly */
	all2=_mm256_set_epi32(2,2,2,2,2,2,2,2);
	/* we can also set all elements to the same value */
	all3=_mm256_set1_epi32(3);
	all4=_mm256_set1_epi32(4);
	all5=_mm256_set1_epi32(5);
	/* we can also set to zero */
	zero=_mm256_setzero_si256();
	vsum=_mm256_setzero_si256();
	for(i=0;i<N;i+=8) {
		/* compare numbers modulo 3 and 5 to 0 and set result vector to true (-1)
		   or false (0) */
		res3=_mm256_cmpeq_epi32(mod3,zero);
		res5=_mm256_cmpeq_epi32(mod5,zero);
		/* or result vectors together. now a vector element is true (-1) if it is
		   divisible by either 3 or 5 */
		res3=_mm256_or_si256(res3,res5);
		/* if element i in res3 is true, then the i-th element is taken from val,
		   otherwise it's taken from zero. unfortunately, even though blendv is
		   content-oblivious, it expects float so we have to cast. in assembly
		   this would look nicer. */
		res3=(__m256i)_mm256_blendv_ps((__m256)zero,(__m256)val,(__m256)res3);
		/* extend 4 first elements in res to 64-bit. here we'd read directly from
		   xmm instead of calling extracti128, let's hope the compiler sees what's
		   going on */
		res5=_mm256_cvtepi32_epi64(_mm256_extracti128_si256(res3,0));
		/* add them to our running sum */
		vsum=_mm256_add_epi64(vsum,res5);
		/* extend 4 last elements in res to 64-bit */
		res5=_mm256_cvtepi32_epi64(_mm256_extracti128_si256(res3,1));
		/* add them to our running sum */
		vsum=_mm256_add_epi64(vsum,res5);

		/* increase val by 8 */
		val=_mm256_add_epi32(val,all8);

		/* increase mod3 by 2 (which is 8 modulo 3) */
		mod3=_mm256_add_epi32(mod3,all2);
		/* elements in res3 are -1 (true) for elements in mod3 > 2 */
		res3=_mm256_cmpgt_epi32(mod3,all2);
		/* mod3 - 3 for all elements */
		res5=_mm256_sub_epi32(mod3,all3);
		/* mod3 - 3 only where res3=true, ugly casting again */
		mod3=(__m256i)_mm256_blendv_ps((__m256)mod3,(__m256)res5,(__m256)res3);

		/* increase mod5 by 3 (which is 8 modulo 5) */
		mod5=_mm256_add_epi32(mod5,all3);
		/* elements in res3 are -1 (true) for elements in mod5 > 4 */
		res3=_mm256_cmpgt_epi32(mod5,all4);
		/* mod5 - 5 for all elements */
		res5=_mm256_sub_epi32(mod5,all5);
		/* mod5 - 5 only where res3=true, ugly casting again */
		mod5=(__m256i)_mm256_blendv_ps((__m256)mod5,(__m256)res5,(__m256)res3);
	}
	/* sum of sum vector */
	return ((ll *)&vsum)[0]+((ll *)&vsum)[1]+((ll *)&vsum)[2]+((ll *)&vsum)[3];
}

int main() {
	double tid;
	ll ans;
	tid=-gettime(); ans=pe001(); tid+=gettime();
	printf("naive:        "LONG" (%.3f s)\n",ans,tid);
	tid=-gettime(); ans=pe001intr1(); tid+=gettime();
	printf("intrinsics 1: "LONG" (%.3f s)\n",ans,tid);
	tid=-gettime(); ans=pe001intr2(); tid+=gettime();
	printf("intrinsics 2: "LONG" (%.3f s)\n",ans,tid);
	tid=-gettime(); ans=pe001intr3(); tid+=gettime();
	printf("intrinsics 3: "LONG" (%.3f s)\n",ans,tid);
	return 0;
}
