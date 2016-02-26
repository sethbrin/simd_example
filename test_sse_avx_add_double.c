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
// in L1 cache. 32K/(sizeof(double)*2) = 2048, divide 2 for the safe
#define BUFSIZE 2048
__attribute__((aligned(32))) double nums[BUFSIZE];

#define LOOP 4000
typedef double (*TESTPROC)(const double* pbuf, int cntbuf);

double sum_base(const double* nums, int cnt)
{
  double res = 0;
  int ind = 0;

  for (ind=0; ind<cnt; ind++)
  {
    res += nums[ind];
  }

  return res;
}

#ifdef __SSE__
double sum_sse(const double* nums, int cnt)
{
  int block_size = 2; // as the sse is 128 bit, 128 = 2 * 64
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m128d res_m128 = _mm_setzero_pd();
  __m128d tmp_m128;

  double res;

  int ind = 0;
  const double* p = nums;
  const double* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m128 = _mm_load_pd(p);
    res_m128 = _mm_add_pd(res_m128, tmp_m128);

    p += block_size;
  }

  q = (const double*)&res_m128;
  res = q[0] + q[1];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}

double sum_sse_unroll_4(const double* nums, int cnt)
{
  int block_size = 4 * 2; // as the sse is 128 bit, 128 = 2 * 64, and unroll 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m128d res_m128 = _mm_setzero_pd();
  __m128d res1_m128 = _mm_setzero_pd();
  __m128d res2_m128 = _mm_setzero_pd();
  __m128d res3_m128 = _mm_setzero_pd();
  __m128d tmp_m128;
  __m128d tmp1_m128;
  __m128d tmp2_m128;
  __m128d tmp3_m128;

  double res;

  int ind = 0;
  const double* p = nums;
  const double* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m128 = _mm_load_pd(p);
    res_m128 = _mm_add_pd(res_m128, tmp_m128);
    tmp1_m128 = _mm_load_pd(p+2);
    res1_m128 = _mm_add_pd(res1_m128, tmp1_m128);
    tmp2_m128 = _mm_load_pd(p+4);
    res2_m128 = _mm_add_pd(res2_m128, tmp2_m128);
    tmp3_m128 = _mm_load_pd(p+6);
    res3_m128 = _mm_add_pd(res3_m128, tmp3_m128);

    p += block_size;
  }

  res_m128 = _mm_add_pd(res_m128, res1_m128);
  res_m128 = _mm_add_pd(res_m128, res2_m128);
  res_m128 = _mm_add_pd(res_m128, res3_m128);

  q = (const double*)&res_m128;
  res = q[0] + q[1];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}
#endif

#ifdef __AVX__
double sum_avx(const double* nums, int cnt)
{
  int block_size = 4; // as the avx is 256 bit, 256 = 4 * 64
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m256d res_m256 = _mm256_setzero_pd();
  __m256d tmp_m256;

  double res;

  int ind = 0;
  const double* p = nums;
  const double* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m256 = _mm256_load_pd(p);
    res_m256 = _mm256_add_pd(res_m256, tmp_m256);

    p += block_size;
  }

  q = (const double*)&res_m256;
  res = q[0] + q[1] + q[2] + q[3];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}

double sum_avx_unroll_4(const double* nums, int cnt)
{
  int block_size = 4 * 4; // as the avx is 256 bit, 256 = 4 * 64, unroll is 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m256d res_m256 = _mm256_setzero_pd();
  __m256d res1_m256 = _mm256_setzero_pd();
  __m256d res2_m256 = _mm256_setzero_pd();
  __m256d res3_m256 = _mm256_setzero_pd();
  __m256d tmp_m256;
  __m256d tmp1_m256;
  __m256d tmp2_m256;
  __m256d tmp3_m256;

  double res;

  int ind = 0;
  const double* p = nums;
  const double* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m256 = _mm256_load_pd(p);
    res_m256 = _mm256_add_pd(res_m256, tmp_m256);
    tmp1_m256 = _mm256_load_pd(p+4);
    res1_m256 = _mm256_add_pd(res1_m256, tmp1_m256);
    tmp2_m256 = _mm256_load_pd(p+8);
    res2_m256 = _mm256_add_pd(res2_m256, tmp2_m256);
    tmp3_m256 = _mm256_load_pd(p+12);
    res3_m256 = _mm256_add_pd(res3_m256, tmp3_m256);

    p += block_size;
  }

  res_m256 = _mm256_add_pd(res_m256, res1_m256);
  res_m256 = _mm256_add_pd(res_m256, res2_m256);
  res_m256 = _mm256_add_pd(res_m256, res3_m256);

  q = (const double*)&res_m256;
  res = q[0] + q[1] + q[2] + q[3];

  for (ind=0; ind<block_remain; ind++)
  {
    res += p[ind];
  }

  return res;
}
#endif

void run_test(const char* proc_name, TESTPROC proc)
{
  int ind;
  double res;
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
    nums[i] = (double)(rand() & 0x3f);
  }

  // test
  // sum_base: 64505.000000  0.035021s
  // sum_sse: 64505.000000 0.024364s
  // sum_sse_unroll_4: 64505.000000  0.013423s
  // sum_avx: 64505.000000 0.015302s
  // sum_avx_unroll_4: 64505.000000  0.007587s
  run_test("sum_base", sum_base);

#ifdef __SSE__
  run_test("sum_sse", sum_sse);
  run_test("sum_sse_unroll_4", sum_sse_unroll_4);
#endif

#ifdef __AVX__
  run_test("sum_avx", sum_avx);
  run_test("sum_avx_unroll_4", sum_avx_unroll_4);
#endif
  return 0;
}
