#include "NUS_depth_buffer.h"
#include "../gpu/NUS_gpu.h"
#include "../gpu/NUS_queue_info.h"
#include "NUS_vk_format.h"
#include <stdio.h>

static NUS_result nus_depth_buffer_find_format(NUS_gpu, VkFormat *);

NUS_result nus_depth_buffer_build
(NUS_queue_info queue_info, unsigned int width, unsigned int height,
 NUS_depth_buffer *p_depth_buffer)
{
  VkFormat format;
  if(nus_depth_buffer_find_format(*queue_info.p_gpu, &format) != NUS_SUCCESS){
    printf("NUS_ERROR::failed to find format for depth buffer\n");
    return NUS_FAILURE;
  }
  if(nus_texture_build(queue_info, width, height, format,
		       VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_depth_buffer) !=
     NUS_SUCCESS){
    printf("NUS_ERROR::failed to create texture for depth buffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
void nus_depth_buffer_free(NUS_gpu gpu, NUS_depth_buffer *p_depth_buffer)
{
  nus_texture_free(gpu, p_depth_buffer);
}
static NUS_result nus_depth_buffer_find_format(NUS_gpu gpu, VkFormat *format)
{
  VkFormat possible_formats[] = {
    VK_FORMAT_D32_SFLOAT, // Comment this line out to require stencil component
    VK_FORMAT_D32_SFLOAT_S8_UINT,
    VK_FORMAT_D24_UNORM_S8_UINT
  };
  unsigned int possible_format_count =
    sizeof(possible_formats) / sizeof(*possible_formats);
  if(nus_vk_format_find_supported(gpu, possible_formats, possible_format_count,
				  VK_IMAGE_TILING_OPTIMAL,
				  VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
				  format) != NUS_SUCCESS){
    printf("NUS_ERROR::failed to find supported format for depth buffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
