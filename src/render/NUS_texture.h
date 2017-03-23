#ifndef NUS_TEXTURE_H
#define NUS_TEXTURE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

struct NUS_gpu;

typedef struct NUS_texture{
  VkImage image;
  VkDeviceMemory memory;
} NUS_texture;

NUS_result nus_texture_build
(struct NUS_gpu, unsigned int, unsigned int, VkFormat, unsigned int, NUS_texture *);
void nus_texture_free(struct NUS_gpu, NUS_texture *);
NUS_result nus_texture_load_image_file(NUS_texture *, char *);


#endif /* NUS_TEXTURE_H */
