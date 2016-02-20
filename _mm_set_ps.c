#include <xmmintrin.h>
#include <stdio.h>

int main()
{
	__m128 value = _mm_set_ps(1.f, 2.f, 3.f, 4.f);

	printf("_mm_set_ps: %f %f %f %f\n", value[0], value[1], value[2], value[3]);

	return 0;
}