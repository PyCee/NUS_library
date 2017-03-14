#ifndef NUS_STRING_GROUP_H
#define NUS_STRING_GROUP_H

#include <stdlib.h>
#include <string.h>
#include "NUS_result.h"

#define nus_string_group_build(p_string_group, ...)			\
  do{									\
    char *nus_string_group_build_array[] = {				\
      __VA_ARGS__							\
    };									\
    unsigned int nus_string_group_build_array_length =			\
      sizeof(nus_string_group_build_array) /				\
      sizeof(*nus_string_group_build_array),				\
      nus_string_group_build_iter;					\
    nus_string_group_init(p_string_group);				\
    for(nus_string_group_build_iter = 0;				\
	nus_string_group_build_iter < nus_string_group_build_array_length; \
	++nus_string_group_build_iter){					\
      nus_string_group_append(p_string_group,				\
			      nus_string_group_build_array		\
			      [nus_string_group_build_iter]);		\
    }									\
  } while(0)

typedef struct NUS_string_group{
  char **strings;
  unsigned int count;
} NUS_string_group;

void nus_string_group_init(NUS_string_group *);
void nus_string_group_free(NUS_string_group *);
void nus_string_group_append(NUS_string_group *, const char *);
NUS_result nus_string_group_set(NUS_string_group *, unsigned int, char const * const);
NUS_result nus_string_group_get(NUS_string_group, unsigned int, char **);
void nus_string_group_print(NUS_string_group);
void nus_string_group_copy(NUS_string_group *, NUS_string_group);
unsigned int nus_string_group_string_index(NUS_string_group, const char *);

#endif /* NUS_STRING_GROUP_H */
