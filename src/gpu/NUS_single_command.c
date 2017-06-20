#include "NUS_single_command.h"
#include "../NUS_vulkan.h"
#include "../NUS_log.h"
#include "NUS_binding.h"

static struct {
  VkCommandBuffer buffer;
  VkCommandBufferBeginInfo begin_info;
  NUS_binding binding;
} nus_single_command;

NUS_result nus_single_command_begin(VkCommandBuffer *p_command_buffer)
{
  if(nus_single_command.buffer == NULL){
    if(nus_allocate_command_buffer(&nus_single_command.buffer, 1) != NUS_SUCCESS){
      NUS_LOG_ERROR("failed to allocate the single command buffer\n");
      return NUS_FAILURE;
    }
    nus_single_command.binding = nus_get_binding();
    nus_single_command.begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = NULL,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = NULL
    };
  }
  vkBeginCommandBuffer(nus_single_command.buffer, &nus_single_command.begin_info);
  *p_command_buffer = nus_single_command.buffer;
  return NUS_SUCCESS;
}
NUS_result nus_single_command_end(void)
{
  nus_bind_binding(&nus_single_command.binding);
  
  if(vkEndCommandBuffer(nus_single_command.buffer) != VK_SUCCESS){
      NUS_LOG_ERROR("failed to record single command buffer\n");
      return NUS_FAILURE;
  }
  nus_add_command_buffer(nus_single_command.buffer);
  nus_submit_queue();
  nus_wait_on_queue();
  vkResetCommandBuffer(nus_single_command.buffer, 0);
  
  nus_unbind_binding(&nus_single_command.binding);
  return NUS_SUCCESS;
}
