#ifndef NUS_CLOCK_H
#define NUS_CLOCK_H

#include "../NUS_os.h"

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
#include <time.h>
struct timespec;
#endif /* OS specifics */

typedef struct NUS_clock{
#if defined(NUS_OS_WINDOWS)
  
#elif defined(NUS_OS_UNIX)
  struct timespec start,
    current;
#endif
} NUS_clock;

void nus_clock_build(NUS_clock *);
#if defined(NUS_OS_WINDOWS)
#elif defined(NUS_OS_UNIX)
void nus_clock_set_start(NUS_clock *, struct timespec);
#endif
double nus_clock_update(NUS_clock *);
double nus_clock_elapsed(NUS_clock);


#endif /* NUS_CLOCK_H */
