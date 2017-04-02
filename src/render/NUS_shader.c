#include "NUS_shader.h"
#include "../gpu/NUS_gpu.h"
#include "../NUS_executable_path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NUS_result nus_shader_build
(NUS_gpu gpu, char *source_file_path, NUS_shader *p_shader)
{
  int i,
    shader_length;
  char *shader_binary;
  FILE *source_file;
  char *absolute_source_file_path = nus_executable_path_prefix(source_file_path);
  if((source_file = fopen(absolute_source_file_path, "r")) == NULL){
    printf("ERROR::failed to open shader file \"%s\"\n", absolute_source_file_path);
    return NUS_FAILURE;
  }
  shader_length = 0;
  while((unsigned long int)getc(source_file) != EOF){
    ++shader_length;
  }
  shader_binary = malloc(sizeof(*shader_binary) * (long unsigned int)shader_length);
  rewind(source_file);
  for(i = 0; i < shader_length; ++i){
    shader_binary[i] = (char)getc(source_file);
  }
  fclose(source_file);


  // replace the code surrounded by comments with a call to nus_shader_build_source
  VkShaderModuleCreateInfo shader_module_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = (long unsigned int)shader_length,
    .pCode = (unsigned int *)shader_binary
  };
  if(vkCreateShaderModule(gpu.logical_device, &shader_module_create_info,
			  NULL, &p_shader->module) != VK_SUCCESS){
    printf("ERROR::failed to create shader module from \"%s\"\n",
	   absolute_source_file_path);
    return NUS_FAILURE;
  }
  //

  
  free(absolute_source_file_path);
  return NUS_SUCCESS;
}
NUS_result nus_shader_build_source(NUS_gpu gpu, char *source, NUS_shader *p_shader)
{
  VkShaderModuleCreateInfo shader_module_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = (long unsigned int)strlen(source),
    .pCode = (unsigned int *)source
  };
  if(vkCreateShaderModule(gpu.logical_device, &shader_module_create_info,
			  NULL, &p_shader->module) != VK_SUCCESS){
    printf("ERROR::failed to create shader module from below code:\n%s\n",
	   source);
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
void nus_shader_free(NUS_gpu gpu, NUS_shader *p_shader)
{
  vkDestroyShaderModule(gpu.logical_device, p_shader->module, NULL);
}
