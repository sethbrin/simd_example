#include <immintrin.h>
#include <stdio.h>

int main()
{
  int i;
	__attribute((aligned(32))) char array[32] = {1,29,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  __m256i* parray = (__m256i*)array;

  __m256i value1 = _mm256_load_si256(parray);

  char* value = (char*)&value1;

  printf("value:");
  for (i=0; i<32; i++)
  {
    printf("%d  ", value[i]);
  }
  printf("\n");
#define __max_16(ret, xx) do { \
      (xx) = _mm256_max_epu8((xx), _mm256_srli_si256((xx), 16)); \
      (xx) = _mm256_max_epu8((xx), _mm256_srli_si256((xx), 8)); \
      (xx) = _mm256_max_epu8((xx), _mm256_srli_si256((xx), 4)); \
      (xx) = _mm256_max_epu8((xx), _mm256_srli_si256((xx), 2)); \
      (xx) = _mm256_max_epu8((xx), _mm256_srli_si256((xx), 1)); \
        (ret) = _mm256_extract_epi16((xx), 0) & 0x00ff; \
    } while (0)

  int res;
  __max_16(res, value1);
  printf("%d\n", res);

	return 0;
}
