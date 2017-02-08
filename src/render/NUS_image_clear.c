#include "NUS_image_clear.h"
#include "../gpu/NUS_gpu.h"
#include <limits.h>
#include <stdio.h>

NUS_result nus_image_clear
(VkSemaphore wait, VkSemaphore signal, VkClearColorValue clear_color,
 NUS_gpu NUS_gpu_, VkImage image_to_clear)
{
  unsigned int queue_family_index = UINT_MAX,
    queue_index = UINT_MAX;
  VkCommandBuffer command_buffer;
  
  if((nus_gpu_find_suitable_queue_family(NUS_gpu_,
					 NUS_QUEUE_FAMILY_SUPPORT_PRESENT |
					 NUS_QUEUE_FAMILY_SUPPORT_TRANSFER,
					 &queue_family_index) !=
      NUS_SUCCESS) || (UINT_MAX == queue_family_index)){
    printf("ERROR::failed to find presentation surface suitable queue family\n");
    return NUS_FAILURE;
  }
  if((nus_queue_family_find_suitable_queue(NUS_gpu_.queue_families[queue_family_index],
					   &queue_index) !=
      NUS_SUCCESS) || (UINT_MAX == queue_index)){
    printf("ERROR::failed to find presentation surface suitable queue\n");
    return NUS_FAILURE;
  }
  NUS_command_queue *command_queue =
    NUS_gpu_.queue_families[queue_family_index].queues + queue_index;
  
  if(nus_command_queue_add_buffer(command_queue,
				  NUS_gpu_.logical_device,
				  NUS_gpu_.queue_families
				  [queue_family_index].command_pool,
				  &command_buffer) != NUS_SUCCESS){
    printf("ERROR::failed to add command queue buffer\n");
    return NUS_FAILURE;
  }
  
  if(nus_command_queue_add_semaphores(command_queue, 1, &wait, 1, &signal) !=
     NUS_SUCCESS){
    printf("ERROR::failed to add semaphores in clear presentation surface\n");
    return NUS_FAILURE;
  }
  VkCommandBufferBeginInfo command_buffer_begin_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    NULL,
    VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
    NULL
  };
  VkImageSubresourceRange image_subresource_range = {
    VK_IMAGE_ASPECT_COLOR_BIT,
    0,
    1,
    0,
    1
  };
  VkImageMemoryBarrier barrier_from_present_to_clear = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
    .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    .srcQueueFamilyIndex = queue_family_index,
    .dstQueueFamilyIndex = queue_family_index,
    .image = image_to_clear,
    .subresourceRange = image_subresource_range
  };
  VkImageMemoryBarrier barrier_from_clear_to_present = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
    .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .srcQueueFamilyIndex = queue_family_index,
    .dstQueueFamilyIndex = queue_family_index,
    .image = image_to_clear,
    .subresourceRange = image_subresource_range
  };
  vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
  
  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		       VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL,
		       1, &barrier_from_present_to_clear);
  
  vkCmdClearColorImage(command_buffer, image_to_clear,
		       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		       &clear_color, 1, &image_subresource_range);
  
  vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL,
		       0, NULL, 1, &barrier_from_clear_to_present);
  
  if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS){
      printf("ERROR::Could not record command buffers!\n");
      return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
