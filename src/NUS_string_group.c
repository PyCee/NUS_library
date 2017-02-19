#include "NUS_string_group.h"

#include <stdio.h>

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
void nus_string_group_print(NUS_string_group NUS_string_group_)
{
  unsigned int i;
  printf("NUS_string_group contains %d strings:\n", NUS_string_group_.count);
  for(i = 0; i < NUS_string_group_.count; ++i){
    printf("  string %d: %s\n", i, NUS_string_group_.strings[i]);
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
char nus_string_group_contains
(NUS_string_group NUS_string_group_, const char *string)
{
  unsigned int i;
  for(i = 0; i < NUS_string_group_.count; ++i){
    if(0 == strcmp(NUS_string_group_.strings[i], string)){
      return 1;
    }
  }
  return 0;
}
