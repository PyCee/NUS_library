#ifndef NUS_FRAMEBUFFER_H
#define NUS_FRAMEBUFFER_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_binding.h"

struct NUS_render_pass;
struct NUS_texture;

#define NUS_FRAMEBUFFER_ATTACHMENT_COUNT 10

typedef struct NUS_framebuffer{
  VkFramebuffer vk_framebuffer;
  VkImageView p_views[NUS_FRAMEBUFFER_ATTACHMENT_COUNT];
  unsigned int view_count,
    width,
    height;
  NUS_binding binding;
} NUS_framebuffer;

void nus_framebuffer_build(unsigned int, unsigned int, unsigned int, NUS_framebuffer *);
NUS_result nus_framebuffer_set_attachment
(unsigned int, struct NUS_texture, unsigned int, NUS_framebuffer *);
NUS_result nus_framebuffer_compile(VkRenderPass, NUS_framebuffer *);
void nus_framebuffer_free(NUS_framebuffer *);

#endif /* NUS_FRAMEBUFFER_H */
