handy reference: http://software.intel.com/sites/landingpage/IntrinsicsGuide/

summary of header files
including a specific file automatically includes all previous extensions

<mmintrin.h>  MMX
<xmmintrin.h> SSE
<emmintrin.h> SSE2
<pmmintrin.h> SSE3
<tmmintrin.h> SSSE3
<smmintrin.h> SSE4.1, including POPCNT
<nmmintrin.h> SSE4.2
<ammintrin.h> SSE4A (AMD)
<wmmintrin.h> AES, PCLNUL
<immintrin.h> AVX, AVX2, including BMI1, BMI2
<zmmintrin.h> AVX512, SVML, KNC

<x86intrin.h> everything (from both intel and amd)

warning, there are no vectorized integer division or modulo instructions

quick overview:

MMX:
- 8 registers, MM0-MM7 (aliased with x87 FPU stack registers)
- packed 64-bit integer instructions

SSE:
- 8 registers, XMM0-XMM7 (16 in 64-bit mode)
- packed 128-bit floating point instructions
- 64-bit movemask

SSE2:
- packed 128-bit integer instructions, including bitwise operators
- 128-bit shuffle
- 128-bit movemask

SSE3:

SSSE3:

SSE4.1:
- 128-bit blend

SSE4.2:

AVX:
- new registers
- packed 256-bit instructions
- 128-bit permute
- 256-bit movemask for float/double

AVX2:
- 256-bit blend
- 256-bit permute
- 256-bit shuffle (only within 128-bit lanes)
- gather
- 256-bit movemask for char
