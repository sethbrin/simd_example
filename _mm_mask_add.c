#include <xmmintrin.h>
#include <stdio.h>

void print128(__m128i val) {
  int8_t* t = (int8_t*) &val;

  int i;
  for (i = 0; i < 16; i++) {
    fprintf(stderr, "%d ", t[i]);
  }
  fprintf(stderr, "\n");
}


int main()
{
  int8_t m[16] = {45, 37, 35, 45, 37, 35, 45, 37, 35, 45, 37, 35,  0, 0,  0, 0};
  int8_t s[16] = {-4, -4, -4, -4,  1,  1, -4, -4, -4, -4,  1,  1, -4,-4, -4, -4};

  __m128i M = _mm_load_si128((__m128i*)m);
  __m128i S = _mm_load_si128((__m128i*)s);
  __m128i zero = _mm_set1_epi32(0);

  //__m128i sum = _mm_mask_add_epi8(zero, _mm_cmpneq_epi8_mask(M, zero), S,S);
  __m128i sum = _mm_andnot_si128(_mm_cmpeq_epi8(M, zero), _mm_add_epi8(M, S));
  print128(M);
  print128(S);
  print128(sum);

  return 0;
}
