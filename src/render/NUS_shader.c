#include "NUS_shader.h"
#include "../NUS_log.h"
#include <stdlib.h>
#include <string.h>

NUS_result nus_shader_build
(NUS_absolute_path absolute_path, VkShaderStageFlagBits stage,
 NUS_shader *p_shader)
{
  int i,
    shader_length;
  char *shader_binary;
  FILE *source_file;
  if((source_file = fopen(absolute_path.path, "r")) == NULL){
    NUS_LOG_ERROR("failed to open shader file \"%s\"\n", absolute_path.path);
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

  p_shader->stage = stage;
  
  VkShaderModuleCreateInfo shader_module_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = (long unsigned int)shader_length,
    .pCode = (unsigned int *)shader_binary
  };
  if(vkCreateShaderModule(nus_get_bound_device(), &shader_module_create_info,
			  NULL, &p_shader->module) != VK_SUCCESS){
    printf("ERROR::failed to create shader module from \"%s\"\n",
	   absolute_path.path);
    return NUS_FAILURE;
  }
  p_shader->binding = nus_get_binding();
  return NUS_SUCCESS;
}
void nus_shader_free(NUS_shader *p_shader)
{
  nus_bind_binding(&p_shader->binding);
  vkDestroyShaderModule(nus_get_bound_device(), p_shader->module, NULL);
  nus_unbind_binding(&p_shader->binding);
}
