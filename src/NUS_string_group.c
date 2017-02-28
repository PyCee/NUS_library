#include "NUS_string_group.h"

#include <stdio.h>
#include <limits.h>

void nus_string_group_init(NUS_string_group *p_NUS_string_group_)
{
  p_NUS_string_group_->strings = NULL;
  p_NUS_string_group_->count = 0;
}
void nus_string_group_free(NUS_string_group *p_NUS_string_group_)
{
  unsigned int i;
  if(0 < p_NUS_string_group_->count){
    for(i = 0; i < p_NUS_string_group_->count; ++i){
      free(p_NUS_string_group_->strings[i]);
    }
    free(p_NUS_string_group_->strings);
    p_NUS_string_group_->strings = NULL;
    p_NUS_string_group_->count = 0;
  }
}
void nus_string_group_append
(NUS_string_group *p_NUS_string_group_, const char *string)
{
  p_NUS_string_group_->strings = realloc(p_NUS_string_group_->strings,
					 sizeof(*p_NUS_string_group_->strings) *
					 ++p_NUS_string_group_->count);
  p_NUS_string_group_->strings[p_NUS_string_group_->count - 1] =
    malloc(sizeof(*string) * (strlen(string) + 1));
  strcpy(p_NUS_string_group_->strings[p_NUS_string_group_->count - 1], string);
}
NUS_result nus_string_group_set
(NUS_string_group *p_NUS_string_group_, unsigned int index, const char * const string)
{
  if(index >= p_NUS_string_group_->count){
    printf("ERROR::cannot set string with index >= count");
    return NUS_FAILURE;
  }
  p_NUS_string_group_->strings[index] = realloc(p_NUS_string_group_->strings[index],
						strlen(string) + 1);
  strcpy(p_NUS_string_group_->strings[index], string);
  return NUS_SUCCESS;
}
NUS_result nus_string_group_get
(NUS_string_group NUS_string_group_, unsigned int index, char **result)
{
  if(index >= NUS_string_group_.count){
    printf("ERROR::failed to resolve string group index %d::greater than count\n",
	   index);
    return NUS_FAILURE;
  }
  *result = NUS_string_group_.strings[index];
  return NUS_SUCCESS;
}
void nus_string_group_print(NUS_string_group NUS_string_group_)
{
  unsigned int i;
  printf("NUS_string_group contains %d string(s):\n", NUS_string_group_.count);
  for(i = 0; i < NUS_string_group_.count; ++i){
    printf("\tstring %d: %s\n", i, NUS_string_group_.strings[i]);
  }
}
void nus_string_group_copy
(NUS_string_group *p_NUS_string_group_dest,
 NUS_string_group NUS_string_group_src)
{
  unsigned int i;
  p_NUS_string_group_dest->strings = NULL;
  p_NUS_string_group_dest->count = 0;
  for(i = 0; i < NUS_string_group_src.count; ++i){
    nus_string_group_append(p_NUS_string_group_dest,
			    NUS_string_group_src.strings[i]);
  }
}
unsigned int nus_string_group_string_index
(NUS_string_group NUS_string_group_, const char *string)
{
  unsigned int i;
  for(i = 0; i < NUS_string_group_.count; ++i){
    if(0 == strcmp(NUS_string_group_.strings[i], string)){
      return i;
    }
  }
  return UINT_MAX;
}
