#ifndef NUS_ABSOLUTE_PATH_H
#define NUS_ABSOLUTE_PATH_H

#define NUS_ABSOLUTE_PATH_MAX_STRING_COUNT 100

typedef struct NUS_absolute_path{
  char path[NUS_ABSOLUTE_PATH_MAX_STRING_COUNT];
} NUS_absolute_path;

NUS_absolute_path nus_absolute_path_build(char const * const);

#endif /* NUS_ABSOLUTE_PATH_H */
