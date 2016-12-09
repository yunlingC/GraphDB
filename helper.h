#ifndef _HELPER_H_
#define _HELPER_H_

#include <time.h>

#define SCALE 1000
#define NANO 1000000000
#define MILLION 1000000

uint64_t get_clock()  {
  timespec tp = (struct timespec) {0};
  clock_gettime(CLOCK_REALTIME, &tp);
  uint64_t ret = tp.tv_sec * NANO + tp.tv_nsec;
  return ret;
}

uint64_t diff_clock(uint64_t  start, uint64_t end) {
  return (end - start) / SCALE;
}

float getPercent(uint64_t top, uint64_t base) {
  return (float)(100 * ((float) top/ (float)(base)));
}

#endif /*_HELPER_H_*/
