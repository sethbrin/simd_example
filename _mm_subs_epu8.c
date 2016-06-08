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
  int8_t m[16] = {45, 37, 35, 45, 37, 35, 45, 37, 35, 45, 37, 35,  0, 0,  0, -1};

  __m128i M = _mm_load_si128((__m128i*)m);
  __m128i oe_ins = _mm_set1_epi8(2);

  print128(M);

  M = _mm_subs_epu8(M, oe_ins);
  print128(M);

  return 0;
}
