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
  int i;
	__attribute((aligned(32))) int16_t array[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  __m256i a = _mm256_load_si256((__m256i*)array);
  __m256i b = _mm256_set1_epi16((int16_t)40);

  print256(a, "a");
  print256(b, "b");
  __m256i hi = _mm256_permute2x128_si256(b,a,0x21);
  print256(hi, "hi");
  hi = _mm256_alignr_epi8(a,hi,14);
  print256(hi, "hi");


	return 0;
}
