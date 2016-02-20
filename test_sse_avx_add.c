//
// test the base, sse, avx version of sum of a vector
//
// Author: Ping Zeng(zengping10212044@gmail.com)
//
// Note: `cat /proc/cpuinfo` first to see if you computer support sse or avx

#include <stdio.h>
#include <immintrin.h>

#include "include/get_time.h"

// As in my machine, L1 cache size is 32K, in order to get ride of the
// influence of memery band, we set the data size which can be contains
// in L1 cache. 32K/(sizeof(float)*2) = 4096, divide 2 for the safe
#define BUFSIZE 4096
__attribute__((aligned(32))) float nums[BUFSIZE];

#define LOOP 4000
typedef float (*TESTPROC)(const float* pbuf, int cntbuf);

float sum_base(const float* nums, int cnt)
{
  float res = 0;
  int ind = 0;

#pragma simd
  for (ind=0; ind<cnt; ind++)
  {
    res += nums[ind];
  }

  return res;
}

float sum_sse(const float* nums, int cnt)
{
  int block_size = 4; // as the sse is 128 bit, 128 = 4 * 32
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m128 res_m128 = _mm_setzero_ps();
  __m128 tmp_m128;

  float res;

  int ind = 0;
  const float* p = nums;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m128 = _mm_load_ps(p);
    res_m128 = _mm_add_ps(res_m128, tmp_m128);

    p += block_size;
  }

  res = res_m128[0] + res_m128[1] + res_m128[2] + res_m128[3];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}

float sum_sse_unroll_4(const float* nums, int cnt)
{
  int block_size = 4 * 4; // as the sse is 128 bit, 128 = 4 * 32, and unroll 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m128 res_m128 = _mm_setzero_ps();
  __m128 res1_m128 = _mm_setzero_ps();
  __m128 res2_m128 = _mm_setzero_ps();
  __m128 res3_m128 = _mm_setzero_ps();
  __m128 tmp_m128;
  __m128 tmp1_m128;
  __m128 tmp2_m128;
  __m128 tmp3_m128;

  float res;

  int ind = 0;
  const float* p = nums;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m128 = _mm_load_ps(p);
    res_m128 = _mm_add_ps(res_m128, tmp_m128);
    tmp1_m128 = _mm_load_ps(p+4);
    res1_m128 = _mm_add_ps(res1_m128, tmp1_m128);
    tmp2_m128 = _mm_load_ps(p+8);
    res2_m128 = _mm_add_ps(res2_m128, tmp2_m128);
    tmp3_m128 = _mm_load_ps(p+12);
    res3_m128 = _mm_add_ps(res3_m128, tmp3_m128);

    p += block_size;
  }

  res_m128 = _mm_add_ps(res_m128, res1_m128);
  res_m128 = _mm_add_ps(res_m128, res2_m128);
  res_m128 = _mm_add_ps(res_m128, res3_m128);
  res = res_m128[0] + res_m128[1] + res_m128[2] + res_m128[3];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}


float sum_avx(const float* nums, int cnt)
{
  int block_size = 8; // as the avx is 256 bit, 256 = 8 * 32
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m256 res_m256 = _mm256_setzero_ps();
  __m256 tmp_m256;

  float res;

  int ind = 0;
  const float* p = nums;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m256 = _mm256_load_ps(p);
    res_m256 = _mm256_add_ps(res_m256, tmp_m256);

    p += block_size;
  }

  res = res_m256[0] + res_m256[1] + res_m256[2] + res_m256[3]
      + res_m256[4] + res_m256[5] + res_m256[6] + res_m256[7];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}

float sum_avx_unroll_4(const float* nums, int cnt)
{
  int block_size = 8 * 4; // as the avx is 256 bit, 256 = 8 * 32, unroll is 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m256 res_m256 = _mm256_setzero_ps();
  __m256 res1_m256 = _mm256_setzero_ps();
  __m256 res2_m256 = _mm256_setzero_ps();
  __m256 res3_m256 = _mm256_setzero_ps();
  __m256 tmp_m256;
  __m256 tmp1_m256;
  __m256 tmp2_m256;
  __m256 tmp3_m256;

  float res;

  int ind = 0;
  const float* p = nums;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m256 = _mm256_load_ps(p);
    res_m256 = _mm256_add_ps(res_m256, tmp_m256);
    tmp1_m256 = _mm256_load_ps(p+8);
    res1_m256 = _mm256_add_ps(res1_m256, tmp1_m256);
    tmp2_m256 = _mm256_load_ps(p+16);
    res2_m256 = _mm256_add_ps(res2_m256, tmp2_m256);
    tmp3_m256 = _mm256_load_ps(p+24);
    res3_m256 = _mm256_add_ps(res3_m256, tmp3_m256);

    p += block_size;
  }

  res_m256 = _mm256_add_ps(res_m256, res1_m256);
  res_m256 = _mm256_add_ps(res_m256, res2_m256);
  res_m256 = _mm256_add_ps(res_m256, res3_m256);

  res = res_m256[0] + res_m256[1] + res_m256[2] + res_m256[3]
      + res_m256[4] + res_m256[5] + res_m256[6] + res_m256[7];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}

void run_test(const char* proc_name, TESTPROC proc)
{
  int ind;
  float res;
  int ind_outer;

  float time = 0.0f;
  float tmp = 0.0f;

  int test_cnt = 3;
  for (ind_outer=0; ind_outer<test_cnt; ind_outer++)
  {
    start_record_time();
    for (ind=0; ind<LOOP; ind++)
    {
      res = proc(nums, BUFSIZE);
    }
    tmp = end_record_time();

    // get the time
    time += tmp;
  }

  printf("%s: %f\t%fs\n", proc_name, res, time/test_cnt);
}

int main()
{
  srand( (unsigned)time( NULL ) );
  int i;
  for (i = 0; i < BUFSIZE; i++)
  {
    nums[i] = (float)(rand() & 0x3f);
  }

  // test
  run_test("sum_base", sum_base);
  run_test("sum_sse", sum_sse);
  run_test("sum_sse_unroll_4", sum_sse_unroll_4);
  run_test("sum_avx", sum_avx);
  run_test("sum_avx_unroll_4", sum_avx_unroll_4);
  return 0;
}
