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
// in L1 cache. 32K/(sizeof(int)*2) = 4096, divide 2 for the safe
#define BUFSIZE 4096
__attribute__((aligned(32))) int nums[BUFSIZE];

#define LOOP 4000
typedef int (*TESTPROC)(const int* pbuf, int cntbuf);

int sum_base(const int* nums, int cnt)
{
  int res = 0;
  int ind = 0;

  for (ind=0; ind<cnt; ind++)
  {
    res += nums[ind];
  }

  return res;
}

int sum_mmx(const int* nums, int cnt)
{
  int block_size = 2; // as the sse is 64 bit, 64 = 2 * 32
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m64 res_m64 = _mm_setzero_si64();
  __m64 tmp_m64;

  int res;

  int ind = 0;
  const __m64* p = (__m64*)nums;
  const int* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m64 = *p;
    res_m64 = _mm_add_pi32(res_m64, tmp_m64);

    p ++;
  }

  q = (const int32_t*)&res_m64;
  res = q[0] + q[1];

  q = (int*)p;
  for (ind=0; ind<block_remain; ind++)
  {
    res += q[ind];
  }

  return res;
}

int sum_mmx_unroll_4(const int* nums, int cnt)
{
  int block_size = 2 * 4; // as the sse is 64 bit, 64 = 2 * 32, and unroll 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m64 res_m64 = _mm_setzero_si64();
  __m64 res1_m64 = _mm_setzero_si64();
  __m64 res2_m64 = _mm_setzero_si64();
  __m64 res3_m64 = _mm_setzero_si64();
  __m64 tmp_m64;
  __m64 tmp1_m64;
  __m64 tmp2_m64;
  __m64 tmp3_m64;

  int res;

  int ind = 0;
  const __m64* p = (__m64*)nums;
  const int* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m64 = *p;
    res_m64 = _mm_add_pi32(res_m64, tmp_m64);
    tmp1_m64 = *(p+1);
    res1_m64 = _mm_add_pi32(res1_m64, tmp1_m64);
    tmp2_m64 = *(p+2);
    res2_m64 = _mm_add_pi32(res2_m64, tmp2_m64);
    tmp3_m64 = *(p+3);
    res3_m64 = _mm_add_pi32(res3_m64, tmp3_m64);

    p +=4;
  }

  res_m64 = _mm_add_pi32(res_m64, res1_m64);
  res_m64 = _mm_add_pi32(res_m64, res2_m64);
  res_m64 = _mm_add_pi32(res_m64, res3_m64);

  // the double and float can use [] to access each number of the m64, but int can not
  q = (const int32_t*)&res_m64;
  res = q[0] + q[1];

  q = (int*)p;
  for (ind=0; ind<block_remain; ind++)
  {
    res += q[ind];
  }

  return res;
}

int sum_sse(const int* nums, int cnt)
{
  int block_size = 4; // as the sse is 128 bit, 128 = 4 * 32
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m128i res_m128 = _mm_setzero_si128();
  __m128i tmp_m128;

  int res;

  int ind = 0;
  const __m128i* p = (__m128i*)nums;
  const int* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m128 = _mm_load_si128(p);
    res_m128 = _mm_add_epi32(res_m128, tmp_m128);

    p ++;
  }

  q = (const int32_t*)&res_m128;
  res = q[0] + q[1] + q[2] + q[3];

  q = (int*)p;
  for (ind=0; ind<block_remain; ind++)
  {
    res += q[ind];
  }

  return res;
}

int sum_sse_unroll_4(const int* nums, int cnt)
{
  int block_size = 4 * 4; // as the sse is 128 bit, 128 = 4 * 32, and unroll 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m128i res_m128 = _mm_setzero_si128();
  __m128i res1_m128 = _mm_setzero_si128();
  __m128i res2_m128 = _mm_setzero_si128();
  __m128i res3_m128 = _mm_setzero_si128();
  __m128i tmp_m128;
  __m128i tmp1_m128;
  __m128i tmp2_m128;
  __m128i tmp3_m128;

  int res;

  int ind = 0;
  const __m128i* p = (__m128i*)nums;
  const int* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m128 = _mm_load_si128(p);
    res_m128 = _mm_add_epi32(res_m128, tmp_m128);
    tmp1_m128 = _mm_load_si128(p+1);
    res1_m128 = _mm_add_epi32(res1_m128, tmp1_m128);
    tmp2_m128 = _mm_load_si128(p+2);
    res2_m128 = _mm_add_epi32(res2_m128, tmp2_m128);
    tmp3_m128 = _mm_load_si128(p+3);
    res3_m128 = _mm_add_epi32(res3_m128, tmp3_m128);

    p +=4;
  }

  res_m128 = _mm_add_epi32(res_m128, res1_m128);
  res_m128 = _mm_add_epi32(res_m128, res2_m128);
  res_m128 = _mm_add_epi32(res_m128, res3_m128);

  // the double and float can use [] to access each number of the m128, but int can not
  q = (int*)&res_m128;
  res = q[0] + q[1] + q[2] + q[3];

  q = (int*)p;
  for (ind=0; ind<block_remain; ind++)
  {
    res += q[ind];
  }

  return res;
}


int sum_avx(const int* nums, int cnt)
{
  int block_size = 8; // as the avx is 256 bit, 256 = 8 * 32
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m256i res_m256 = _mm256_setzero_si256();
  __m256i tmp_m256;

  int res;

  int ind = 0;
  const __m256i* p = (__m256i*)nums;
  const int* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m256 = _mm256_load_si256(p);
    res_m256 = _mm256_add_epi32(res_m256, tmp_m256);

    p ++;
  }

  q = (const int*)&res_m256;
  res = q[0] + q[1] + q[2] + q[3]
      + q[4] + q[5] + q[6] + q[7];

  q = (int*)p;
  for (ind=0; ind<block_remain; ind++)
  {
    res += q[ind];
  }

  return res;
}

int sum_avx_unroll_4(const int* nums, int cnt)
{
  int block_size = 8 * 4; // as the avx is 256 bit, 256 = 8 * 32, unroll is 4
  int block_cnt = cnt / block_size;
  int block_remain = cnt % block_size;

  __m256i res_m256 = _mm256_setzero_si256();
  __m256i res1_m256 = _mm256_setzero_si256();
  __m256i res2_m256 = _mm256_setzero_si256();
  __m256i res3_m256 = _mm256_setzero_si256();
  __m256i tmp_m256;
  __m256i tmp1_m256;
  __m256i tmp2_m256;
  __m256i tmp3_m256;

  int res;

  int ind = 0;
  const __m256i* p = (__m256i*)nums;
  const int* q;

  for (ind=0; ind<block_cnt; ind++)
  {
    tmp_m256 = _mm256_load_si256(p);
    res_m256 = _mm256_add_epi32(res_m256, tmp_m256);
    tmp1_m256 = _mm256_load_si256(p+1);
    res1_m256 = _mm256_add_epi32(res1_m256, tmp1_m256);
    tmp2_m256 = _mm256_load_si256(p+2);
    res2_m256 = _mm256_add_epi32(res2_m256, tmp2_m256);
    tmp3_m256 = _mm256_load_si256(p+3);
    res3_m256 = _mm256_add_epi32(res3_m256, tmp3_m256);

    p += 4;
  }

  res_m256 = _mm256_add_epi32(res_m256, res1_m256);
  res_m256 = _mm256_add_epi32(res_m256, res2_m256);
  res_m256 = _mm256_add_epi32(res_m256, res3_m256);

  q = (const int*)&res_m256;
  res = q[0] + q[1] + q[2] + q[3]
      + q[4] + q[5] + q[6] + q[7];

  q = (int*)p;
  for (ind=0; ind<block_remain; ind++)
  {
    res += q[ind];
  }

  return res;
}

void run_test(const char* proc_name, TESTPROC proc)
{
  int ind;
  int res;
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

  printf("%s: %d\t%fs\n", proc_name, res, time/test_cnt);
}

int main()
{
  srand( (unsigned)time( NULL ) );
  int i;
  for (i = 0; i < BUFSIZE; i++)
  {
    nums[i] = (int)(((float)rand())/RAND_MAX * 10 + 1);
  }

  // test
  // sum_base: 22535 0.069715s
  // sum_mmx: 22535  0.038848s
  // sum_mmx_unroll_4: 22535 0.020536s
  // sum_sse: 22535  0.024179s
  // sum_sse_unroll_4: 22535 0.012925s
  // sum_avx: 22535  0.013203s
  // sum_avx_unroll_4: 22535 0.007119s
  run_test("sum_base", sum_base);
  run_test("sum_mmx", sum_mmx);
  run_test("sum_mmx_unroll_4", sum_mmx_unroll_4);
  run_test("sum_sse", sum_sse);
  run_test("sum_sse_unroll_4", sum_sse_unroll_4);
  run_test("sum_avx", sum_avx);
  run_test("sum_avx_unroll_4", sum_avx_unroll_4);
  return 0;
}
