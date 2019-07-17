#include "commons.h"

// This is not thread safe!!

// Current time, microseconds since the epoch
uint64_t now() {
  THRIFT_GETTIMEOFDAY(&tv, NULL);
  int64_t ret;
  ret = tv.tv_sec;
  ret = ret * 1000 * 1000 + tv.tv_usec;
  return ret;
}


void sw_start(stopwatch* sw) {
  sw->start = now();
}

void sw_stop(stopwatch* sw) {
  sw->stop = now();
  sw->total += sw->stop - sw->start;
  sw->count++;
}

double sw_getAVG(stopwatch sw){
  return (sw.total * 1.0) / sw.count;
}


