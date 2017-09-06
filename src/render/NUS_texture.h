#ifndef NUS_TEXTURE_H
#define NUS_TEXTURE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"
#include "../gpu/NUS_binding.h"

struct NUS_gpu;
struct NUS_queue_info;

typedef struct NUS_texture{
  VkImage image;
  VkDeviceMemory memory;
  VkFormat format;
  VkImageAspectFlags aspect_flags;
  unsigned int width, height;
  uint32_t image_size;
  NUS_binding binding;
} NUS_texture;

NUS_result nus_texture_build
(unsigned int, unsigned int, VkFormat, VkImageUsageFlags, VkMemoryPropertyFlags,
 VkImageAspectFlags, NUS_texture *);
void nus_texture_free(NUS_texture *);
NUS_result nus_texture_buffer_image
(void *, size_t, NUS_texture *);
NUS_result nus_cmd_texture_transition
(NUS_texture, VkCommandBuffer, VkImageLayout, VkImageLayout);


#endif /* NUS_TEXTURE_H */
