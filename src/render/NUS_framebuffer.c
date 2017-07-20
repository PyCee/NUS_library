#include "NUS_framebuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include "../gpu/NUS_gpu.h"
#include "NUS_render_pass.h"
#include "NUS_texture.h"
#include "NUS_image_view.h"
#include "../NUS_log.h"
#include <string.h>

NUS_result nus_framebuffer_build
(NUS_framebuffer *p_framebuffer, NUS_render_pass render_pass,
 NUS_image_view *p_image_views, unsigned int view_count,
 unsigned int width, unsigned int height)
{
  int i;
  VkFramebufferCreateInfo framebuffer_create_info;
  VkImageView vk_image_views[view_count];
  
  *p_framebuffer = (NUS_framebuffer){
    .vk_framebuffer = VK_NULL_HANDLE,
    .binding = nus_get_binding()
  };
  for(i = 0; i < view_count; ++i){
    vk_image_views[i] = p_image_views[i].vk_image_view;
  }
  framebuffer_create_info = (VkFramebufferCreateInfo){
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = render_pass.vk_render_pass,
    .attachmentCount = view_count,
    .pAttachments = vk_image_views,
    .width = width,
    .height = height,
    .layers = 1
  };
  if(vkCreateFramebuffer(nus_get_bound_device(), &framebuffer_create_info, NULL,
			 &p_framebuffer->vk_framebuffer) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create framebuffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}

void nus_framebuffer_free(NUS_framebuffer *p_framebuffer)
{
  nus_bind_binding(&p_framebuffer->binding);
  if(p_framebuffer->vk_framebuffer != VK_NULL_HANDLE){
    vkDestroyFramebuffer(nus_get_bound_device(), p_framebuffer->vk_framebuffer, NULL);
    p_framebuffer->vk_framebuffer = VK_NULL_HANDLE;
  }
  nus_unbind_binding(&p_framebuffer->binding);
}
