#include <immintrin.h>
#include <stdio.h>
#include <stdint.h>

void print256(__m256i val, char* str) {
  int i;
  int16_t* value = (int16_t*)&val;
  printf("%s:", str);
  for (i = 0; i < 16; i++) {
    printf("%d ", value[i]);
  }
  printf("\n");
}

int main()
{

	__attribute((aligned(32))) int16_t array1[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	__attribute((aligned(32))) int16_t array2[16] = {3,2,1,4,5,6,7,8,9,10,11,12,13,14,15,16};
  __m256i a = _mm256_load_si256((__m256i*)array1);
  __m256i b = _mm256_load_si256((__m256i*)array2);

  print256(a, "a");
  print256(b, "b");

  __m256i c =  _mm256_and_si256(_mm256_cmpgt_epi16(a, b), _mm256_set1_epi16(1));
  print256(c, "c");
  __m256i d = _mm256_set1_epi16(5);
  __m256i mask = _mm256_cmpgt_epi16(a, d);
  print256(mask, "mask");
  c = _mm256_or_si256(
                      _mm256_and_si256(mask, _mm256_set1_epi16(2)),
                      _mm256_andnot_si256(mask, c)
                                                                                                    );
  print256(c, "c");
  return 0;
}
