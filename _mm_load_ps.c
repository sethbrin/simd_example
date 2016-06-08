#include <xmmintrin.h>
#include <stdio.h>

int main()
{
	float array[4] = {1.f, 2.f, 3.f, 4.f};

	__m128 value1 = _mm_load_ps(array);

        float* value = (float*)&value1;

	printf("_mm_load_ps: %f %f %f %f\n", value[0], value[1], value[2], value[3]);

	return 0;
}
