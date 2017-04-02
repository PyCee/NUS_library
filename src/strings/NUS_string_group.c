#include "NUS_string_group.h"

#include <stdio.h>
#include <limits.h>

void nus_string_group_build(NUS_string_group *p_string_group)
{
  p_string_group->strings = NULL;
  p_string_group->count = 0;
}
void nus_string_group_free(NUS_string_group *p_string_group)
{
  unsigned int i;
  if(0 < p_string_group->count){
    for(i = 0; i < p_string_group->count; ++i){
      free(p_string_group->strings[i]);
    }
    free(p_string_group->strings);
    p_string_group->strings = NULL;
    p_string_group->count = 0;
  }
}
void nus_string_group_append
(NUS_string_group *p_string_group, const char *string)
{
  p_string_group->strings = realloc(p_string_group->strings,
					 sizeof(*p_string_group->strings) *
					 ++p_string_group->count);
  p_string_group->strings[p_string_group->count - 1] =
    malloc(sizeof(*string) * (strlen(string) + 1));
  strcpy(p_string_group->strings[p_string_group->count - 1], string);
}
NUS_result nus_string_group_set
(NUS_string_group *p_string_group, unsigned int index, const char * const string)
{
  if(index >= p_string_group->count){
    printf("ERROR::failed to resolve string group index %d::greater than count\n",
	   index);
    return NUS_FAILURE;
  }
  p_string_group->strings[index] = realloc(p_string_group->strings[index],
						strlen(string) + 1);
  strcpy(p_string_group->strings[index], string);
  return NUS_SUCCESS;
}
NUS_result nus_string_group_get
(NUS_string_group string_group, unsigned int index, char **result)
{
  if(index >= string_group.count){
    printf("ERROR::failed to resolve string group index %d::greater than count\n",
	   index);
    return NUS_FAILURE;
  }
  *result = string_group.strings[index];
  return NUS_SUCCESS;
}
void nus_string_group_copy
(NUS_string_group *p_string_group_dest,
 NUS_string_group string_group_src)
{
  unsigned int i;
  for(i = 0; i < string_group_src.count; ++i){
    nus_string_group_append(p_string_group_dest,
			    string_group_src.strings[i]);
  }
}
unsigned int nus_string_group_string_index
(NUS_string_group string_group, const char *string)
{
  unsigned int i;
  for(i = 0; i < string_group.count; ++i){
    if(0 == strcmp(string_group.strings[i], string)){
      return i;
    }
  }
  return UINT_MAX;
}
void nus_string_group_print(NUS_string_group string_group)
{
  unsigned int i;
  printf("NUS_string_group contains %d string(s):\n", string_group.count);
  for(i = 0; i < string_group.count; ++i){
    printf("\tstring %d: %s\n", i, string_group.strings[i]);
  }
}
