#ifndef NUS_RENDER_PASS_H
#define NUS_RENDER_PASS_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"
#include "../gpu/NUS_binding.h"

typedef struct NUS_render_pass{
  VkRenderPass vk_render_pass;
  NUS_binding binding;
} NUS_render_pass;

NUS_result nus_render_pass_build(VkRenderPassCreateInfo, NUS_render_pass *);
void nus_render_pass_free(NUS_render_pass *);
VkRenderPass nus_render_pass_get_raw(NUS_render_pass);

#endif /* NUS_RENDER_PASS_H */
