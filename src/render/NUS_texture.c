#include "NUS_texture.h"
#include "../gpu/NUS_gpu.h"
#include "../gpu/NUS_queue_info.h"
#include "../gpu/NUS_memory_properties.h"
#include "../NUS_log.h"
#include <string.h>

NUS_result nus_texture_layout_to_mask(VkImageLayout, unsigned int *);

NUS_result nus_texture_build
(NUS_gpu gpu, unsigned int width, unsigned int height,
 VkFormat format, unsigned int usage, unsigned int memory_property_flag,
 NUS_texture *p_texture)
{
  p_texture->format = format;
  VkImageCreateInfo image_create_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .imageType = VK_IMAGE_TYPE_2D,
    .format = p_texture->format,
    .extent = {
      .width = width,
      .height = height,
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
  if(vkCreateImage(gpu.logical_device, &image_create_info, NULL,
		   &p_texture->image) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create texture image\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements image_memory_req;
  vkGetImageMemoryRequirements(gpu.logical_device, p_texture->image, &image_memory_req);
  VkMemoryAllocateInfo memory_alloc_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = image_memory_req.size,
    .memoryTypeIndex =
    nus_memory_properties_type_index(gpu, image_memory_req, memory_property_flag)
  };
  if(vkAllocateMemory(gpu.logical_device, &memory_alloc_info, NULL,
		      &p_texture->memory) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to allocate memory for texture\n");
    return NUS_FAILURE;
  }
  vkBindImageMemory(gpu.logical_device, p_texture->image, p_texture->memory, 0);
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
NUS_result nus_texture_load_data
(NUS_gpu gpu, NUS_texture *p_texture, void *src, size_t size)
{
  void* data;
  vkMapMemory(gpu.logical_device, p_texture->memory, 0, size, 0, &data);
  memcpy(data, src, size);
  vkUnmapMemory(gpu.logical_device, p_texture->memory);
  
  return NUS_SUCCESS;
}
NUS_result nus_texture_initial_transition
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
  
  VkCommandBuffer command_buffer;
  nus_queue_info_add_buffer(queue_info, &command_buffer);
  
  VkCommandBufferBeginInfo command_buffer_begin_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    .pInheritanceInfo = NULL
  };
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
  vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		       0, 0, NULL, 0, NULL, 1, &barrier_from_old_to_new);
  if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS){
      NUS_LOG_ERROR("Could not record command buffer!\n");
      return NUS_FAILURE;
  }
  nus_command_group_append(queue_info.p_command_group, command_buffer);
  nus_queue_info_submit(queue_info);
  
  return NUS_SUCCESS;
}

NUS_result nus_texture_layout_to_mask(VkImageLayout layout, unsigned int *mask)
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
