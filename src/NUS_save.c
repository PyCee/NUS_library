#include "NUS_save.h"
#include <stdio.h>
#include "NUS_executable_path.h"

NUS_result nus_save_build(char const * const file_path, NUS_save *p_NUS_save_)
{
  nus_string_group_init(&p_NUS_save_->contents);
  FILE *file;
  char *absolute_path = nus_executable_path_prefix(file_path);
  if((file = fopen(absolute_path, "r")) == NULL){
    printf("ERROR::failed to open save file \"%s\" to read\n", absolute_path);
    return NUS_FAILURE;
  }
  free(absolute_path);

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
    nus_string_group_append(&p_NUS_save_->contents, element);
    nus_string_group_append(&p_NUS_save_->contents, attribute);
  }
  
  fclose(file);
  return NUS_SUCCESS;
}
NUS_result nus_save_output(NUS_save NUS_save_, char const * const file_path)
{
  FILE *file;
  char *absolute_path = nus_executable_path_prefix(file_path);
  if((file = fopen(absolute_path, "w")) == NULL){
    printf("ERROR::failed to open save file \"%s\" to read\n", absolute_path);
    return NUS_FAILURE;
  }
  free(absolute_path);
  
  unsigned int i;

  fprintf(file, "%s\n", NUS_SAVE_FILE_HEAD);
  for(i = 0; i < NUS_save_.contents.count; i += 2){
    fprintf(file, "%s:%s\n", NUS_save_.contents.strings[i],
	    NUS_save_.contents.strings[i + 1]);
  }
  fclose(file);
  return NUS_SUCCESS;
}
void nus_save_free(NUS_save *p_NUS_save_)
{
  nus_string_group_free(&p_NUS_save_->contents);
}
NUS_result nus_save_set_variable
(NUS_save *p_NUS_save_, char const * const element, char const * const attribute)
{
  unsigned int element_index = nus_string_group_string_index(p_NUS_save_->contents,
							     element);
  if(nus_string_group_set(&p_NUS_save_->contents, element_index + 1, attribute) !=
     NUS_SUCCESS){
    printf("ERROR::failed to get save attribute\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_save_get_attribute
(NUS_save NUS_save_, char const * const element, char **attribute)
{
  unsigned int element_index = nus_string_group_string_index(NUS_save_.contents,
							     element);
  if(nus_string_group_get(NUS_save_.contents, element_index + 1, attribute) !=
     NUS_SUCCESS){
    printf("ERROR::failed to get save attribute\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
