#include "NUS_render_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include "../NUS_log.h"

NUS_result nus_render_pass_build
(VkRenderPassCreateInfo create_info, NUS_render_pass *p_render_pass)
{
  if(vkCreateRenderPass(nus_get_bound_device(), &create_info, NULL,
			&p_render_pass->vk_render_pass) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create vulkan render pass\n");
    return NUS_FAILURE;
  }
  p_render_pass->binding = nus_get_binding();
  return NUS_SUCCESS;
}
void nus_render_pass_free(NUS_render_pass *p_render_pass)
{
  nus_bind_binding(&p_render_pass->binding);
  vkDestroyRenderPass(nus_get_bound_device(), p_render_pass->vk_render_pass, NULL);
  p_render_pass->vk_render_pass = VK_NULL_HANDLE;
  nus_unbind_binding(&p_render_pass->binding);
}
VkRenderPass nus_render_pass_get_raw(NUS_render_pass render_pass)
{
  return render_pass.vk_render_pass;
}
