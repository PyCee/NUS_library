#include "NUS_save.h"
#include <stdio.h>
#include <limits.h>

NUS_result nus_save_build(NUS_absolute_path absolute_path, NUS_save *p_save)
{
  nus_string_group_init(&p_save->contents);
  FILE *file;
  if((file = fopen(absolute_path.path, "r")) == NULL){
    printf("ERROR::failed to open save file \"%s\" to read\n", absolute_path.path);
    return NUS_FAILURE;
  }

  char file_line[NUS_SAVE_FILE_MAX_LINE_LENGTH],
    element[NUS_SAVE_FILE_MAX_LINE_LENGTH],
    attribute[NUS_SAVE_FILE_MAX_LINE_LENGTH];
  fscanf(file, "%s", file_line);
  if(0 != strcmp(NUS_SAVE_FILE_HEAD, file_line)){
    printf("ERROR::file does not begin with required head \"%s\"\n",
	   NUS_SAVE_FILE_HEAD);
    return NUS_FAILURE;
  }
  char *seperator;
  while(fscanf(file, "%s\n", file_line) != EOF){
    memset(element, '\0', NUS_SAVE_FILE_MAX_LINE_LENGTH);
    memset(attribute, '\0', NUS_SAVE_FILE_MAX_LINE_LENGTH);
    if((seperator = strstr(file_line, ":")) == NULL){
      printf("ERROR::no occurence of \":\"in file line %s\n", file_line);
      return NUS_FAILURE;
    }
    memcpy(element, file_line, (size_t)(seperator - file_line));
    element[seperator - file_line] = '\0';
    
    memcpy(attribute, seperator + 1,
	   (size_t)((file_line + strlen(file_line)) - (seperator + 1)));
    nus_string_group_append(&p_save->contents, element);
    nus_string_group_append(&p_save->contents, attribute);
  }
  
  fclose(file);
  return NUS_SUCCESS;
}
NUS_result nus_save_output(NUS_absolute_path absolute_path, NUS_save save)
{
  unsigned int i;
  FILE *file;
  if((file = fopen(absolute_path.path, "w")) == NULL){
    printf("ERROR::failed to open save file \"%s\" to read\n", absolute_path.path);
    return NUS_FAILURE;
  }

  fprintf(file, "%s\n", NUS_SAVE_FILE_HEAD);
  for(i = 0; i < save.contents.count; i += 2){
    fprintf(file, "%s:%s\n", save.contents.strings[i],
	    save.contents.strings[i + 1]);
  }
  fclose(file);
  return NUS_SUCCESS;
}
void nus_save_free(NUS_save *p_save)
{
  nus_string_group_free(&p_save->contents);
}
NUS_result nus_save_set_variable
(NUS_save *p_save, char const * const element, char const * const attribute)
{
  unsigned int element_index;
  if((element_index = nus_string_group_string_index(p_save->contents,
						    element)) == UINT_MAX){
    printf("ERROR::failed to get save element string index to set\n");
    return NUS_FAILURE;
  }
  if(nus_string_group_set(&p_save->contents, element_index + 1, attribute) !=
     NUS_SUCCESS){
    printf("ERROR::failed to get save attribute\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_save_get_attribute
(NUS_save save, char const * const element, char **attribute)
{
  unsigned int element_index;
  if((element_index = nus_string_group_string_index(save.contents,
						    element)) == UINT_MAX){
    printf("ERROR::failed to get save element string index\n");
    return NUS_FAILURE;
  }
  if(nus_string_group_get(save.contents, element_index + 1, attribute) !=
     NUS_SUCCESS){
    printf("ERROR::failed to get save attribute\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
