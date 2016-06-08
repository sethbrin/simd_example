#include <immintrin.h>
#include <stdio.h>

int main()
{
  int i;
	__attribute((aligned(32))) char array[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  __m256i* parray = (__m256i*)array;

  __m256i value1 = _mm256_load_si256(parray);
  __m256i zero = _mm256_set1_epi32(0);

  char* value = (char*)&value1;

  printf("value:");
  for (i=0; i<32; i++)
  {
    printf("%d  ", value[i]);
  }
  printf("\n");
  //value1 = _mm256_slli_si256(value1, 1);
  value1 = _mm256_alignr_epi8(value1, _mm256_permute2x128_si256(value1, value1, _MM_SHUFFLE(0, 0, 2, 0)), 16 - 1);
  //value1 = _mm256_alignr_epi8(value1, zero, 15);
  value = (char*)&value1;
  printf("value:");
  for (i=0; i<32; i++)
  {
    printf("%d  ", value[i]);
  }
  printf("\n");

	return 0;
}
