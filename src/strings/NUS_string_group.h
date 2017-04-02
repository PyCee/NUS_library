#ifndef NUS_STRING_GROUP_H
#define NUS_STRING_GROUP_H

#include <stdlib.h>
#include <string.h>
#include "../NUS_result.h"

typedef struct NUS_string_group{
  char **strings;
  unsigned int count;
} NUS_string_group;

void nus_string_group_build(NUS_string_group *);
void nus_string_group_free(NUS_string_group *);
void nus_string_group_append(NUS_string_group *, const char *);
NUS_result nus_string_group_set(NUS_string_group *, unsigned int, char const * const);
NUS_result nus_string_group_get(NUS_string_group, unsigned int, char **);
void nus_string_group_copy(NUS_string_group *, NUS_string_group);
unsigned int nus_string_group_string_index(NUS_string_group, const char *);
void nus_string_group_print(NUS_string_group);

#endif /* NUS_STRING_GROUP_H */
