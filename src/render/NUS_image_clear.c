#include "NUS_image_clear.h"
#include "../gpu/NUS_gpu.h"
#include <limits.h>
#include <stdio.h>
#include "../NUS_log.h"
#include "../gpu/NUS_binding.h"

NUS_result nus_image_clear
(VkSemaphore wait, VkSemaphore signal, VkClearColorValue clear_color,
 VkImage image_to_clear)
{
  VkCommandBuffer command_buffer;
  
  nus_allocate_command_buffer(&command_buffer, 1);
  nus_add_wait_semaphore(wait, VK_PIPELINE_STAGE_TRANSFER_BIT);
  nus_add_signal_semaphore(signal);
  
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
    .srcQueueFamilyIndex = nus_get_bound_queue_family()->family_index,
    .dstQueueFamilyIndex = nus_get_bound_queue_family()->family_index,
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
    .srcQueueFamilyIndex = nus_get_bound_queue_family()->family_index,
    .dstQueueFamilyIndex = nus_get_bound_queue_family()->family_index,
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

  nus_add_command_buffer(command_buffer);
  nus_submit_queue();
  return NUS_SUCCESS;
}

