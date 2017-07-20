#ifndef NUS_FRAMEBUFFER_H
#define NUS_FRAMEBUFFER_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_binding.h"

struct NUS_render_pass;
struct NUS_texture;
struct NUS_image_view;

typedef struct NUS_framebuffer{
  VkFramebuffer vk_framebuffer;
  NUS_binding binding;
} NUS_framebuffer;

NUS_result nus_framebuffer_build
(NUS_framebuffer *, struct NUS_render_pass, struct NUS_image_view *, unsigned int,
 unsigned int, unsigned int);
void nus_framebuffer_free(NUS_framebuffer *);

#endif /* NUS_FRAMEBUFFER_H */
