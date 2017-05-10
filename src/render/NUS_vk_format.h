#ifndef NUS_VK_FORMAT_H
#define NUS_VK_FORMAT_H

#include "../NUS_vulkan.h"

struct NUS_gpu;

NUS_result nus_vk_format_find_supported
(struct NUS_gpu, VkFormat *, unsigned int, VkImageTiling, VkFormatFeatureFlags,
 VkFormat *);

#endif /* NUS_VK_FORMAT_H */
