#include "NUS_subpass_info.h"

#include <stdlib.h>
#include <stdio.h>

NUS_result nus_subpass_info_build
(unsigned int input_count, unsigned int color_count,
 unsigned int preserve_count, NUS_subpass_info *p_subpass_info)
{
  if(NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT <= input_count){
    printf("ERROR::failed to build subpass info input count %d::max count is %d\n",
	   input_count, NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT);
    return NUS_FAILURE;
  }
  if(NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT <= color_count){
    printf("ERROR::failed to build subpass info color count %d::max count is %d\n",
	   color_count, NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT);
    return NUS_FAILURE;
  }
  if(NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT <= preserve_count){
    printf("ERROR::failed to build subpass info preserve count %d::max count is %d\n",
	   preserve_count, NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT);
    return NUS_FAILURE;
  }
  p_subpass_info->input_ref_count = input_count;
  p_subpass_info->color_ref_count = color_count;
  p_subpass_info->preserve_ref_count = preserve_count;
  return NUS_SUCCESS;
}
void nus_subpass_info_free(NUS_subpass_info *p_subpass_info)
{
}
/*
NUS_result nus_subpass_info_set_input_ref
(unsigned int attachment_index, VkImageLayout layout,
 unsigned int index, NUS_subpass_info *p_subpass_info)
{
  if(index >= p_subpass_info->input_ref_count || index < 0){
    printf("ERROR::invalid subpass info input ref index %d::count is %d\n",
	   index, p_subpass_info->input_ref_count);
    return NUS_FAILURE;
  }
  p_subpass_info->p_input_attachment_refs[index] = (VkAttachmentReference){
    .attachment = attachment_index,
    .layout = layout
  };
  return NUS_SUCCESS;
}
NUS_result nus_subpass_info_set_color_ref
(unsigned int attachment_index, VkImageLayout layout,
 unsigned int index, NUS_subpass_info *p_subpass_info)
{
  if(index >= p_subpass_info->color_ref_count || index < 0){
    printf("ERROR::invalid subpass info color ref index %d::count is %d\n",
	   index, p_subpass_info->color_ref_count);
    return NUS_FAILURE;
  }
  p_subpass_info->p_color_attachment_refs[index] = (VkAttachmentReference){
    .attachment = attachment_index,
    .layout = layout
  };
  return NUS_SUCCESS;
}
NUS_result nus_subpass_info_set_depth_stencil_ref
(unsigned int attachment_index, VkImageLayout layout,
 NUS_subpass_info *p_subpass_info)
{
  p_subpass_info->p_depth_stencil_attachment_ref[0] = (VkAttachmentReference){
    .attachment = attachment_index,
    .layout = layout
  };
  return NUS_SUCCESS;
}
NUS_result nus_subpass_info_set_preserve_ref
(unsigned int attachment_index, VkImageLayout layout,
 unsigned int index, NUS_subpass_info *p_subpass_info)
{
  if(index >= p_subpass_info->preserve_ref_count || index < 0){
    printf("ERROR::invalid subpass info preserve ref index %d::count is %d\n",
	   index, p_subpass_info->preserve_ref_count);
    return NUS_FAILURE;
  }
  p_subpass_info->p_preserve_attachment_refs[index] = (VkAttachmentReference){
    .attachment = attachment_index,
    .layout = layout
  };
  return NUS_SUCCESS;
}
*/
