#include "NUS_image_clear.h"
#include "../gpu/NUS_gpu.h"
#include <limits.h>
#include <stdio.h>
#include "../gpu/NUS_suitable_queue.h"
#include "../gpu/NUS_command_group.h"

NUS_result nus_image_clear
(VkSemaphore wait, VkSemaphore signal, VkClearColorValue clear_color,
 NUS_gpu *NUS_gpu_, VkImage image_to_clear)
{
  VkCommandBuffer command_buffer;

  NUS_suitable_queue info;
  nus_gpu_find_suitable_queue(NUS_gpu_,
			      NUS_QUEUE_FAMILY_SUPPORT_PRESENT,
			      &info);
  
  nus_suitable_queue_add_buffer(info, &command_buffer);
  
  if(nus_command_group_add_semaphores(info.p_command_group, 1, &wait, 1, &signal) !=
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
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    .srcQueueFamilyIndex = info.queue_family_index,
    .dstQueueFamilyIndex = info.queue_family_index,
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
    .srcQueueFamilyIndex = info.queue_family_index,
    .dstQueueFamilyIndex = info.queue_family_index,
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
      printf("ERROR::Could not record command buffer!\n");
      return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
