#include "NUS_single_command.h"
#include "../NUS_vulkan.h"
#include "../NUS_log.h"

VkCommandBuffer nus_single_command_buffer = VK_NULL_HANDLE;
VkCommandBufferBeginInfo nus_single_command_buffer_begin_info;

NUS_result nus_single_command_begin
(NUS_queue_info queue_info, VkCommandBuffer *p_command_buffer)
{
  if(nus_single_command_buffer == VK_NULL_HANDLE){
    nus_queue_info_add_buffer(queue_info, &nus_single_command_buffer);
    nus_single_command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = NULL,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = NULL
    };
  }
  vkBeginCommandBuffer(nus_single_command_buffer, &nus_single_command_buffer_begin_info);
  *p_command_buffer = nus_single_command_buffer;
  return NUS_SUCCESS;
}
NUS_result nus_single_command_end
(NUS_queue_info queue_info)
{
  if(vkEndCommandBuffer(nus_single_command_buffer) != VK_SUCCESS){
      NUS_LOG_ERROR("failed to record single command buffer\n");
      return NUS_FAILURE;
  }
  nus_command_group_append(queue_info.p_command_group, nus_single_command_buffer);
  nus_queue_info_submit(queue_info);
  vkDeviceWaitIdle(queue_info.p_gpu->logical_device);
  vkResetCommandBuffer(nus_single_command_buffer, 0);
  return NUS_SUCCESS;
}
