#ifndef NUS_FRAMEBUFFER_H
#define NUS_FRAMEBUFFER_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

struct NUS_gpu;
struct NUS_render_pass;
struct NUS_texture;

typedef struct NUS_framebuffer_info{
  VkImageViewCreateInfo *p_view_create_infos;
  unsigned int view_count,
    width,
    height;
} NUS_framebuffer_info;

typedef struct NUS_framebuffer{
  VkFramebuffer vk_framebuffer;
  VkImageView *p_views;
  unsigned int view_count;
} NUS_framebuffer;

NUS_result nus_framebuffer_info_build
(unsigned int, unsigned int, unsigned int, NUS_framebuffer_info *);
void nus_framebuffer_info_free(struct NUS_gpu, NUS_framebuffer_info *);
NUS_result nus_framebuffer_info_set_attachment
(struct NUS_gpu, struct NUS_texture, VkFormat, unsigned int,
 unsigned int, NUS_framebuffer_info *);

NUS_result nus_framebuffer_build
(struct NUS_gpu, VkRenderPass, NUS_framebuffer_info, NUS_framebuffer *);
void nus_framebuffer_free(struct NUS_gpu, NUS_framebuffer *);
#endif /* NUS_FRAMEBUFFER_H */
