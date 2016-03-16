#include <immintrin.h>
#include <stdio.h>

int main()
{
	__attribute((aligned(16))) char array[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  __m128i value1 = _mm_load_si128((__m128i*)array);

  char* value = (char*)&value1;

	printf("_mm_slli_si128: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
         value[0], value[1], value[2], value[3],
         value[4], value[5], value[6], value[7],
         value[8], value[9], value[10], value[11],
         value[12], value[13], value[14], value[15]
         );
  value1 = _mm_slli_si128(value1, 1);
  value = (char*)&value1;
	printf("_mm_slli_si128 after left shift: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
         value[0], value[1], value[2], value[3],
         value[4], value[5], value[6], value[7],
         value[8], value[9], value[10], value[11],
         value[12], value[13], value[14], value[15]
         );

	return 0;
}
