#ifndef NUS_LOG_H
#define NUS_LOG_H

#include <stdio.h>

#define NUS_LOG( ... )				\
  printf("NUS_LOG::" __VA_ARGS__)
#define NUS_LOG_ERROR( ... )			\
  NUS_LOG("ERROR::" __VA_ARGS__)
#define NUS_LOG_WARNING( ... )			\
  NUS_LOG("WARNING::" __VA_ARGS__)

#endif /* NUS_LOG_H */
