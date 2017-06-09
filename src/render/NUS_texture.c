#include "NUS_texture.h"
#include "../gpu/NUS_gpu.h"
#include "../gpu/NUS_queue_info.h"
#include "../gpu/NUS_memory_map.h"
#include "../NUS_log.h"
#include "../gpu/NUS_single_command.h"
#include <string.h>

static NUS_result nus_texture_layout_to_mask(VkImageLayout, unsigned int *);

NUS_result nus_texture_build
(NUS_queue_info queue_info, unsigned int width, unsigned int height,
 VkFormat format, unsigned int usage, unsigned int memory_property_flags,
 NUS_texture *p_texture)
{
  p_texture->format = format;
  p_texture->width = width;
  p_texture->height = height;
  
  VkImageCreateInfo image_create_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .imageType = VK_IMAGE_TYPE_2D,
    .format = p_texture->format,
    .extent = {
      .width = p_texture->width,
      .height = p_texture->height,
      .depth = 1
    },
    .mipLevels = 1,
    .arrayLayers = 1,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED
  };
  if(vkCreateImage(queue_info.p_gpu->logical_device, &image_create_info, NULL,
		   &p_texture->image) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create texture image\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements image_memory_req;
  vkGetImageMemoryRequirements(queue_info.p_gpu->logical_device, p_texture->image,
			       &image_memory_req);
  
  VkMemoryAllocateInfo memory_alloc_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = image_memory_req.size,
    .memoryTypeIndex = nus_gpu_memory_type_index(*queue_info.p_gpu, image_memory_req,
						 memory_property_flags)
  };
  if(vkAllocateMemory(queue_info.p_gpu->logical_device, &memory_alloc_info, NULL,
		      &p_texture->memory) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to allocate memory for texture\n");
    return NUS_FAILURE;
  }
  vkBindImageMemory(queue_info.p_gpu->logical_device, p_texture->image,
		    p_texture->memory, 0);
  return NUS_SUCCESS;
}
void nus_texture_free(NUS_gpu gpu, NUS_texture *p_texture)
{
  if(p_texture->memory != VK_NULL_HANDLE){
    vkFreeMemory(gpu.logical_device, p_texture->memory, NULL);
    p_texture->memory = VK_NULL_HANDLE;
  }
  if(p_texture->image != VK_NULL_HANDLE){
    vkDestroyImage(gpu.logical_device, p_texture->image, NULL);
    p_texture->image = VK_NULL_HANDLE;
  }
}
NUS_result nus_texture_buffer_image
(NUS_queue_info queue_info, void *data, size_t size, NUS_texture *p_texture)
{
  p_texture->image_size = size;
  
  NUS_memory_map tmp_memory_map;
  if(nus_memory_map_build(queue_info, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			  &tmp_memory_map) != NUS_SUCCESS){
    NUS_LOG("failed to build memory map for texture\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(tmp_memory_map, queue_info, data) != NUS_SUCCESS){
    NUS_LOG("failed to flush memory map for texture\n");
    return NUS_FAILURE;
  }
  
  
  // copy buffer to image
  VkBufferImageCopy region = {
    .bufferOffset = 0,
    .bufferRowLength = 0,
    .bufferImageHeight = 0,
    .imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    .imageSubresource.mipLevel = 0,
    .imageSubresource.baseArrayLayer = 0,
    .imageSubresource.layerCount = 1,
    .imageOffset = {0, 0, 0},
    .imageExtent = {
      p_texture->width,
      p_texture->height,
      1
    }
  };
  nus_texture_transition(*p_texture, queue_info, VK_IMAGE_LAYOUT_PREINITIALIZED,
			 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			 VK_IMAGE_ASPECT_COLOR_BIT);
  
  VkCommandBuffer command_buffer;
  nus_single_command_begin(queue_info, &command_buffer);
  
  vkCmdCopyBufferToImage(command_buffer, tmp_memory_map.buffer, p_texture->image,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  
  if(nus_single_command_end(queue_info) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to copy texture buffer to image");
    return NUS_FAILURE;
  }
  
  nus_texture_transition(*p_texture, queue_info, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			 VK_IMAGE_ASPECT_COLOR_BIT);

  nus_memory_map_free(&tmp_memory_map, queue_info);
  return NUS_SUCCESS;
}
NUS_result nus_texture_transition
(NUS_texture texture, NUS_queue_info queue_info,
 VkImageLayout src_layout, VkImageLayout dst_layout, unsigned int aspect_mask)
{
  unsigned int src_access_mask,
    dst_access_mask;

  if(nus_texture_layout_to_mask(src_layout, &src_access_mask) != NUS_SUCCESS){
    NUS_LOG_ERROR("invalid src_layout in initial texture transition\n");
    return NUS_FAILURE;
  }
  if(nus_texture_layout_to_mask(dst_layout, &dst_access_mask) != NUS_SUCCESS){
    NUS_LOG_ERROR("invalid dst_layout in initial texture transition\n");
    return NUS_FAILURE;
  }
  VkImageSubresourceRange image_subresource_range = {
    aspect_mask,
    0,
    1,
    0,
    1
  };
  VkImageMemoryBarrier barrier_from_old_to_new = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = src_access_mask,
    .dstAccessMask = dst_access_mask,
    .oldLayout = src_layout,
    .newLayout = dst_layout,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = texture.image,
    .subresourceRange = image_subresource_range
  };
  VkCommandBuffer command_buffer;
  nus_single_command_begin(queue_info, &command_buffer);
  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		       0, 0, NULL, 0, NULL, 1, &barrier_from_old_to_new);
  if(nus_single_command_end(queue_info) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to end texture transition command buffer\n");
    return NUS_FAILURE;
  }
  
  return NUS_SUCCESS;
}

static NUS_result nus_texture_layout_to_mask(VkImageLayout layout, unsigned int *mask)
{
  switch(layout){
  case VK_IMAGE_LAYOUT_UNDEFINED:
    *mask = 0;
    break;
  case VK_IMAGE_LAYOUT_PREINITIALIZED:
    *mask = VK_ACCESS_HOST_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    *mask = VK_ACCESS_TRANSFER_READ_BIT;
    break;
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    *mask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    *mask = VK_ACCESS_SHADER_READ_BIT;
    break;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    *mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    break;
  default:
    NUS_LOG_ERROR("invalid layout\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
