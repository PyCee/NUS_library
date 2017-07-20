#ifndef NUS_IMAGE_VIEW_H
#define NUS_IMAGE_VIEW_H

#include "../NUS_result.h"
#include "../gpu/NUS_binding.h"
#include "../NUS_vulkan.h"

struct NUS_texture;

typedef struct NUS_image_view{
  VkImageView vk_image_view;
  NUS_binding binding;
} NUS_image_view;

NUS_result nus_image_view_build
(NUS_image_view *, struct NUS_texture, VkImageAspectFlags);
void nus_image_view_free(NUS_image_view *);

#endif /* NUS_IMAGE_VIEW_H */
