#include <time.h>
#include <stdio.h>

// global variable
struct timeval g_get_time_start_time, g_get_time_end_time;

void start_record_time()
{
  gettimeofday(&g_get_time_start_time,NULL);
}

// the return time is xx second
float end_record_time()
{
  gettimeofday(&g_get_time_end_time,NULL);

  return (1000000*(g_get_time_end_time.tv_sec - g_get_time_start_time.tv_sec) + (g_get_time_end_time.tv_usec - g_get_time_start_time.tv_usec))/1000000.0;
}
