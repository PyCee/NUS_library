#ifndef NUS_CLEAR_IMAGE_H
#define NUS_CLEAR_IMAGE_H

#include "../NUS_vulkan.h"

struct NUS_gpu;

NUS_result nus_image_clear
(VkSemaphore, VkSemaphore, VkClearColorValue, struct NUS_gpu, VkImage);

#endif /* NUS_CLEAR_IMAGE_H */
