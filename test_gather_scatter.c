//
// test the gather performance
//
// Author: Ping Zeng(zengping10212044@gmail.com)
//
// Note: `cat /proc/cpuinfo` first to see if you computer support sse or avx

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#include "include/get_time.h"

#define OUTPUT_SIZE 100000
#define INPUT_SIZE 10000

#define LOOP 4000
typedef float (*TESTPROC)();

typedef struct float3 {
  float x;
  float y;
  float z;
} float3;

int idx[OUTPUT_SIZE];
float3 input[INPUT_SIZE];
float output[OUTPUT_SIZE];

// return a random float range [start, end)
float get_random(int start, int end) {
    return ((float)rand()/RAND_MAX * (end - start)) + start;
}

void init()
{
  srand( (unsigned)time( NULL ) );
  int i;
  for (i=0; i<INPUT_SIZE; i++) {
    input[i].x = get_random(1,10);
    input[i].y = get_random(1,10);
    input[i].z = get_random(1,10);
  }
  for (i=0; i<OUTPUT_SIZE; i++) {
    idx[i] = (int)get_random(0, INPUT_SIZE);
  }
}

float base()
{
  int i;
  float total = 0;
  float3 tmp;
  float delx, dely, delz;
  float3 tmp1 = input[0];
  for (i=0; i<OUTPUT_SIZE; i++) {
    tmp = input[idx[i]];

    delx = tmp.x - tmp1.x;
    dely = tmp.y - tmp1.y;
    delz = tmp.z - tmp1.z;

    output[i] = delx*delx + dely*dely + delz*delz;
  }
  for (i=0; i<OUTPUT_SIZE; i++) {
    total += output[i];
  }
  return total;
}

float improve()
{
  int i;
  float total = 0;
  float tmpx, tmpy, tmpz;
  float delx, dely, delz;
  float3 tmp1 = input[0];
  for (i=0; i<OUTPUT_SIZE; i++) {
    tmpx = input[idx[i]].x;
    tmpy = input[idx[i]].y;
    tmpz = input[idx[i]].z;

    delx = tmpx - tmp1.x;
    dely = tmpy - tmp1.y;
    delz = tmpz - tmp1.z;

    output[i] = delx*delx + dely*dely + delz*delz;
  }
  for (i=0; i<OUTPUT_SIZE; i++) {
    total += output[i];
  }
  return total;
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
      res = proc();
    }
    tmp = end_record_time();

    // get the time
    time += tmp;
  }

  printf("%s: %f\t%fs\n", proc_name, res, time/test_cnt);
}

int main()
{
  init();
  // use -march=corei7-avx no gather
  // base: 2694191.500000 0.843764s
  // improve: 2694191.500000 0.659572s
  //
  // use -march=core-avx2 with gather
  // base: 3702585.000000  1.046425s
  // improve: 3702585.000000 0.831004s

  run_test("base", base);
  run_test("improve", improve);
  return 0;
}
