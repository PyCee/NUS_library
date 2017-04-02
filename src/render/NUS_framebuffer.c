#include "NUS_framebuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include "../gpu/NUS_gpu.h"
#include "NUS_render_pass.h"
#include "NUS_texture.h"

NUS_result nus_framebuffer_info_build
(unsigned int width, unsigned int height, unsigned int view_count,
 NUS_framebuffer_info *p_framebuffer_info)
{
  p_framebuffer_info->view_count = view_count;
  p_framebuffer_info->p_view_create_infos =
    malloc(sizeof(*p_framebuffer_info->p_view_create_infos) * view_count);
  p_framebuffer_info->width = width;
  p_framebuffer_info->height = height;
  return NUS_SUCCESS;
}
void nus_framebuffer_info_free
(NUS_gpu gpu, NUS_framebuffer_info *p_framebuffer_info)
{
  if(p_framebuffer_info->p_view_create_infos != NULL){
    free(p_framebuffer_info->p_view_create_infos);
    p_framebuffer_info->p_view_create_infos = NULL;
    p_framebuffer_info->view_count = 0;
  }
}
NUS_result nus_framebuffer_info_set_attachment
(NUS_gpu gpu, NUS_texture texture, VkFormat format, unsigned int aspect_mask,
 unsigned int index, NUS_framebuffer_info *p_framebuffer_info)
{
  if(index >= p_framebuffer_info->view_count || index < 0){
    printf("ERROR::failed to add framebuffer attachment %d::invalid index\n",
	   index);
    return NUS_FAILURE;
  }
  p_framebuffer_info->p_view_create_infos[index] = (VkImageViewCreateInfo){
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = texture.image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .a = VK_COMPONENT_SWIZZLE_IDENTITY
    },
    .subresourceRange = {
      .aspectMask = aspect_mask,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };
  return NUS_SUCCESS;
}

NUS_result nus_framebuffer_build
(NUS_gpu gpu, VkRenderPass render_pass, NUS_framebuffer_info framebuffer_info,
 NUS_framebuffer *p_framebuffer)
{
  unsigned int i;
  VkFramebufferCreateInfo framebuffer_create_info;
  p_framebuffer->view_count = framebuffer_info.view_count;
  p_framebuffer->p_views = malloc(sizeof(*p_framebuffer->p_views) *
					framebuffer_info.view_count);
  for(i = 0; i < framebuffer_info.view_count; ++i){
     if(vkCreateImageView(gpu.logical_device,
			  &framebuffer_info.p_view_create_infos[i], NULL,
			  &p_framebuffer->p_views[i]) != VK_SUCCESS){
       printf("ERROR::failed to create framebuffer image view attachment\n");
       return NUS_FAILURE;
     }
  }
  framebuffer_create_info = (VkFramebufferCreateInfo){
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = render_pass,
    .attachmentCount = framebuffer_info.view_count,
    .pAttachments = p_framebuffer->p_views,
    .width = framebuffer_info.width,
    .height = framebuffer_info.height,
    .layers = 1
  };
  if(vkCreateFramebuffer(gpu.logical_device, &framebuffer_create_info, NULL,
			 &p_framebuffer->vk_framebuffer) != VK_SUCCESS){
    printf("ERROR::failed to create framebuffer vk_framebuffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
void nus_framebuffer_free(NUS_gpu gpu, NUS_framebuffer *p_framebuffer)
{
  unsigned int i;
  if(p_framebuffer->p_views != NULL){
    for(i = 0; i < p_framebuffer->view_count; ++i){
      if(p_framebuffer->p_views[i] != VK_NULL_HANDLE){
	vkDestroyImageView(gpu.logical_device, p_framebuffer->p_views[i], NULL);
	p_framebuffer->p_views[i] = VK_NULL_HANDLE;
      }
    }
    p_framebuffer->p_views = NULL;
    p_framebuffer->view_count = 0;
  }
  if(p_framebuffer->vk_framebuffer != VK_NULL_HANDLE){
    vkDestroyFramebuffer(gpu.logical_device, p_framebuffer->vk_framebuffer, NULL);
    p_framebuffer->vk_framebuffer = VK_NULL_HANDLE;
  }
}
