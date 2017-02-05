#include "NUS_clock.h"

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
#define CURRENT_TIME(timespec_)					\
  clock_gettime(CLOCK_REALTIME, &timespec_)

static double nus_time_difference(struct timespec, struct timespec);

void nus_clock_build(NUS_clock *NUS_clock_)
{
  CURRENT_TIME(NUS_clock_->start);
  NUS_clock_->current = NUS_clock_->start;
}
void nus_clock_set_start(NUS_clock *NUS_clock_, struct timespec start)
{
  NUS_clock_->start = start;
  if((NUS_clock_->current.tv_sec < NUS_clock_->start.tv_sec) ||
     ((NUS_clock_->current.tv_sec == NUS_clock_->start.tv_sec) &&
      (NUS_clock_->current.tv_nsec < NUS_clock_->start.tv_nsec))){
    NUS_clock_->current = NUS_clock_->start;
  }
}
double nus_clock_update(NUS_clock *NUS_clock_)
{
  struct timespec previous = NUS_clock_->current;
  CURRENT_TIME(NUS_clock_->current);
  return nus_time_difference(NUS_clock_->current, previous);
}
double nus_clock_elapsed(NUS_clock NUS_clock_)
{
  return nus_time_difference(NUS_clock_.current, NUS_clock_.start);
}

static double nus_time_difference(struct timespec t1, struct timespec t2)
{
  return (double)(t1.tv_sec - t2.tv_sec) +
    ((double)(t1.tv_nsec - t2.tv_nsec) / 1000000000.0);
}
#endif
