#include <xmmintrin.h>
#include <stdio.h>

int main()
{
	__m128 value1 = _mm_set_ps1(1.f);
    float* value = (float*)&value1;

	printf("_mm_set_ps1: %f %f %f %f\n", value[0], value[1], value[2], value[3]);

	return 0;
}
