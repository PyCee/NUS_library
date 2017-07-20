#include "NUS_framebuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include "../gpu/NUS_gpu.h"
#include "NUS_render_pass.h"
#include "NUS_texture.h"
#include "../NUS_log.h"
#include <string.h>

NUS_result nus_framebuffer_build
(unsigned int view_count, unsigned int width, unsigned int height,
 NUS_framebuffer *p_framebuffer)
{
  p_framebuffer->vk_framebuffer = VK_NULL_HANDLE;
  memset(p_framebuffer->p_views, VK_NULL_HANDLE, sizeof(*p_framebuffer->p_views) *
	 NUS_FRAMEBUFFER_ATTACHMENT_COUNT);
  p_framebuffer->view_count = view_count;
  p_framebuffer->width = width;
  p_framebuffer->height = height;
  p_framebuffer->binding = nus_get_binding();
  return NUS_SUCCESS;
}

NUS_result nus_framebuffer_set_attachment
(unsigned int index, NUS_texture texture, VkImageAspectFlags aspect_mask,
 NUS_framebuffer *p_framebuffer)
{
  if(index >= NUS_FRAMEBUFFER_ATTACHMENT_COUNT || index < 0){
    NUS_LOG_ERROR("framebuffer attachment index \'%d\' not in range of (0 - %d)\n",
		  index, NUS_FRAMEBUFFER_ATTACHMENT_COUNT);
    return NUS_FAILURE;
  }
  if(p_framebuffer->p_views[index] != VK_NULL_HANDLE){
    NUS_LOG_ERROR("attempting to set framebuffer attachment \'%d\' more than once\n",
		  index);
  }
  if(texture.width != p_framebuffer->width){
    NUS_LOG_ERROR("attachment width \'%d\' does not match framebuffer width \'%d\'\n",
		  texture.height, p_framebuffer->height);
    return NUS_FAILURE;
  }
  if(texture.height != p_framebuffer->height){
    NUS_LOG_ERROR("attachment height \'%d\' does not match framebuffer height \'%d\'\n",
		  texture.height, p_framebuffer->height);
    return NUS_FAILURE;
  }
  
  nus_bind_binding(&p_framebuffer->binding);

  VkImageViewCreateInfo view_create_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = texture.image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = texture.format,
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
  if(vkCreateImageView(nus_get_bound_device(), &view_create_info, NULL,
		       &p_framebuffer->p_views[index]) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create framebuffer image view attachment\n");
    return NUS_FAILURE;
  }
  
  nus_unbind_binding(&p_framebuffer->binding);
  return NUS_SUCCESS;
}

NUS_result nus_framebuffer_compile
(NUS_render_pass render_pass, NUS_framebuffer *p_framebuffer)
{
  VkFramebufferCreateInfo create_info;
  
  nus_bind_binding(&p_framebuffer->binding);
  
  create_info = (VkFramebufferCreateInfo){
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = render_pass.vk_render_pass,
    .attachmentCount = p_framebuffer->view_count,
    .pAttachments = p_framebuffer->p_views,
    .width = p_framebuffer->width,
    .height = p_framebuffer->height,
    .layers = 1
  };
  if(vkCreateFramebuffer(nus_get_bound_device(), &create_info, NULL,
			 &p_framebuffer->vk_framebuffer) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create framebuffer\n");
    return NUS_FAILURE;
  }
  
  nus_unbind_binding(&p_framebuffer->binding);
  
  return NUS_SUCCESS;
}
void nus_framebuffer_free(NUS_framebuffer *p_framebuffer)
{
  unsigned int i;
  nus_bind_binding(&p_framebuffer->binding);
  if(p_framebuffer->p_views != NULL){
    for(i = 0; i < p_framebuffer->view_count; ++i){
      if(p_framebuffer->p_views[i] != VK_NULL_HANDLE){
	vkDestroyImageView(nus_get_bound_device(), p_framebuffer->p_views[i], NULL);
	p_framebuffer->p_views[i] = VK_NULL_HANDLE;
      }
    }
  }
  if(p_framebuffer->vk_framebuffer != VK_NULL_HANDLE){
    vkDestroyFramebuffer(nus_get_bound_device(), p_framebuffer->vk_framebuffer, NULL);
    p_framebuffer->vk_framebuffer = VK_NULL_HANDLE;
  }
  nus_unbind_binding(&p_framebuffer->binding);
}
