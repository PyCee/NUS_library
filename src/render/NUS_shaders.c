#include "NUS_shaders.h"
#include "../gpu/NUS_gpu.h"
#include "../io/NUS_executable_path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NUS_result nus_shader_build
(NUS_gpu NUS_gpu_, char *source_file_path, NUS_shader *NUS_shader_)
{
  int i,
    shader_length = 0,
    *shader_binary;

  FILE *source_file;
  
  char *absolute_source_file_path = nus_executable_path_prefix(source_file_path);
  
  if((source_file = fopen(absolute_source_file_path, "r")) == NULL){
    printf("ERROR::failed to open shader file \"%s\"\n", absolute_source_file_path);
    return NUS_FAILURE;
  }
  
  while((unsigned long int)getc(source_file) != EOF){
    ++shader_length;
  }
  
  shader_binary = malloc(sizeof(*shader_binary) * (long unsigned int)shader_length);
  rewind(source_file);
  for(i = 0; i < shader_length; ++i){
    shader_binary[i] = getc(source_file);
  }
  fclose(source_file);

  VkShaderModuleCreateInfo shader_module_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = sizeof(*shader_binary) * (long unsigned int)shader_length,
    .pCode = (unsigned int *)shader_binary
    //if there is a problem, look at .pCode
  };

  if(vkCreateShaderModule(NUS_gpu_.logical_device, &shader_module_create_info,
			  NULL, &NUS_shader_->module) != VK_SUCCESS){
    printf("ERROR::failed to create shader module from \"%s\"\n",
	   absolute_source_file_path);
    return NUS_FAILURE;
  }

  free(absolute_source_file_path);
  
  return NUS_SUCCESS;
}
