#ifndef NUS_TEXTURE_H
#define NUS_TEXTURE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

struct NUS_gpu;
struct NUS_queue_info;

typedef struct NUS_texture{
  VkImage image;
  VkDeviceMemory memory;
  VkFormat format;
  unsigned int width, height;
  size_t image_size;
} NUS_texture;

NUS_result nus_texture_build
(struct NUS_queue_info, unsigned int, unsigned int, VkFormat, unsigned int, unsigned int,
 NUS_texture *);
void nus_texture_free(struct NUS_gpu, NUS_texture *);
NUS_result nus_texture_buffer_image
(struct NUS_queue_info, void *, size_t, NUS_texture *);
NUS_result nus_texture_initial_transition
(NUS_texture, struct NUS_queue_info, VkImageLayout, VkImageLayout, unsigned int);


#endif /* NUS_TEXTURE_H */
