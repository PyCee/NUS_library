#include "NUS_vk_format.h"
#include "../gpu/NUS_gpu.h"
#include <stdio.h>

NUS_result nus_vk_format_find_supported
(NUS_gpu gpu, VkFormat *p_formats, unsigned int format_count,
 VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat *p_format)
{
  unsigned int i;
  VkFormatProperties properties;
  for( i = 0; i < format_count; ++i){
    vkGetPhysicalDeviceFormatProperties(gpu.physical_device, p_formats[i], &properties);
    if((tiling == VK_IMAGE_TILING_LINEAR &&
	(properties.linearTilingFeatures & features) == features) ||
       (tiling == VK_IMAGE_TILING_OPTIMAL &&
	(properties.optimalTilingFeatures & features) == features)){
      *p_format = p_formats[i];
      return NUS_SUCCESS;
    }
  }
  printf("NUS_ERROR::failed to find support for format\n");
  return NUS_FAILURE;
}
