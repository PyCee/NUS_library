#include "NUS_depth_buffer.h"
#include "../gpu/NUS_binding.h"
#include "../NUS_log.h"
#include <stdio.h>

static NUS_result nus_depth_buffer_find_format(VkFormat *);

NUS_result nus_depth_buffer_build
(unsigned int width, unsigned int height,
 NUS_depth_buffer *p_depth_buffer)
{
  VkFormat format;
  if(nus_depth_buffer_find_format(&format) != NUS_SUCCESS){
    printf("NUS_ERROR::failed to find format for depth buffer\n");
    return NUS_FAILURE;
  }
  if(nus_texture_build(width, height, format,
		       VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT,
		       p_depth_buffer) != NUS_SUCCESS){
    printf("NUS_ERROR::failed to create texture for depth buffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_depth_buffer_find_format
(VkFormat *format)
{
  int i;
  VkFormat possible_formats[] = {
    VK_FORMAT_D32_SFLOAT, // Comment this line out to require stencil component
    VK_FORMAT_D32_SFLOAT_S8_UINT,
    VK_FORMAT_D24_UNORM_S8_UINT
  };
  unsigned int possible_format_count =
    sizeof(possible_formats) / sizeof(*possible_formats);
  for(i = 0; i < possible_format_count; ++i){
    if(nus_gpu_qwery_format_support(*nus_get_bound_gpu(), possible_formats[i],
				    VK_IMAGE_TILING_OPTIMAL,
				    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
       == NUS_TRUE){
      *format = possible_formats[i];
      return NUS_SUCCESS;
    }
  }
  printf("NUS_ERROR::failed to find supported format for depth buffer\n");
  return NUS_FAILURE;
}
