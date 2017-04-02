#include "NUS_render_pass.h"
#include "../gpu/NUS_gpu.h"
#include <stdio.h>
#include <stdlib.h>
/*
NUS_result nus_render_pass_info_build
(unsigned int attachment_count, unsigned int subpass_count,
 unsigned int dependency_count,  NUS_render_pass_info *p_render_pass_info)
{
  if(NUS_RENDER_PASS_INFO_MAX_ATTACHMENT_COUNT <= attachment_count){
    printf("ERROR::failed to build render pass info attachment count %d::\
max count is %d\n", attachment_count, NUS_RENDER_PASS_INFO_MAX_ATTACHMENT_COUNT);
    return NUS_FAILURE;
  }
  if(NUS_RENDER_PASS_INFO_MAX_SUBPASS_COUNT <= subpass_count){
    printf("ERROR::failed to build render pass info subpass count %d::\
max count is %d\n", subpass_count, NUS_RENDER_PASS_INFO_MAX_SUBPASS_COUNT);
    return NUS_FAILURE;
  }
  if(NUS_RENDER_PASS_INFO_MAX_DEPENDENCY_COUNT <= dependency_count){
    printf("ERROR::failed to build render pass info dependency count %d::\
max count is %d\n", dependency_count, NUS_RENDER_PASS_INFO_MAX_DEPENDENCY_COUNT);
    return NUS_FAILURE;
  }
  
  p_render_pass_info->attachment_description_count = attachment_count;
  p_render_pass_info->subpass_count = subpass_count;
  p_render_pass_info->dependency_count = dependency_count;
  return NUS_SUCCESS;
}
void nus_render_pass_info_free(NUS_gpu gpu, NUS_render_pass_info *p_render_pass_info)
{
}
*/
/*
NUS_result nus_render_pass_info_set_attachment_description
(NUS_gpu gpu, VkAttachmentDescription attachment_description,
 unsigned int attachment_index, NUS_render_pass_info *p_render_pass_info)
{
  if(attachment_index < 0 ||
     attachment_index >= p_render_pass_info->attachment_description_count){
    printf("ERROR::attachment_index %d is not in render pass info range %d\n",
	   attachment_index, p_render_pass_info->attachment_description_count);
    return NUS_FAILURE;
  }
  p_render_pass_info->p_attachment_descriptions[attachment_info] =
    attachment_description;
  return NUS_SUCCESS;
}
NUS_result nus_render_pass_info_set_subpass
(NUS_subpass_info subpass, unsigned int index,
 NUS_render_pass_info *p_render_pass_info)
{
  if(index >= p_render_pass_info->subpass_count || index < 0){
    printf("ERROR::Invalid render pass subpass index %d::count is %d",
	   index, p_render_pass_info->subpass_count);
    return NUS_FAILURE;
  }
  p_render_pass_info
}
NUS_result nus_render_pass_info_set_dependency
(VkSubpassDependency, unsigned int, NUS_render_pass_info *);
*/
