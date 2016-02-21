//
// test the likely() function in gcc
//
// Author: Ping Zeng(zengping10212044@gmail.com)
//

#include <stdio.h>

#include "include/get_time.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define BUFSIZE 100000
#define THRESHOLD 100

int nums[BUFSIZE];

#define LOOP 40
typedef int (*TESTPROC)(const int* pbuf, const int cntbuf);

int fun(const int* nums, const int cnt)
{
  int ind = 0;
  int res = 0;
  for (ind=0; ind<cnt; ind++)
  {
    if (nums[ind] > THRESHOLD)
    {
      res ++;
    }
    else
    {
      res --;
    }
  }

  return res;
}

int fun_likely(const int* nums, const int cnt)
{
  int ind = 0;
  int res = 0;
  for (ind=0; ind<cnt; ind++)
  {
    if (likely(nums[ind] > THRESHOLD))
    {
      res ++;
    }
    else
    {
      res --;
    }
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
  int i;
  for (i = 0; i < BUFSIZE; i++)
  {
    nums[i] = i;
  }

  // test
  // fun: 99798  0.015020s
  // fun_likely: 99798  0.013998s
  run_test("fun", fun);
  run_test("fun_likely", fun_likely);
  return 0;
}
