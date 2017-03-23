#include "NUS_command_group.h"
#include <stdio.h>
#include <stdlib.h>
#include "../NUS_vulkan.h"
#include "NUS_queue_info.h"

static NUS_result nus_command_group_reset_buffers
(NUS_command_group *);

NUS_result nus_command_group_build
(VkDevice device, unsigned int family_index, unsigned int queue_index,
 NUS_command_group *NUS_command_group_)
{
  vkGetDeviceQueue(device, family_index, queue_index, &NUS_command_group_->queue);
  NUS_command_group_->command_buffers = NULL;
  NUS_command_group_->wait = NULL;
  NUS_command_group_->signal = NULL;
  NUS_command_group_->command_buffer_count = 0;
  NUS_command_group_->wait_count = 0;
  NUS_command_group_->signal_count = 0;
  
  return NUS_SUCCESS;
}
void nus_command_group_free
(NUS_command_group *NUS_command_group_)
{
  // Command buffers are freed when command pool is freed in parent NUS_group_family
  
  NUS_command_group_->command_buffers = NULL;
  NUS_command_group_->command_buffer_count = 0;
  
  NUS_command_group_->wait = NULL;
  NUS_command_group_->wait_count = 0;
  NUS_command_group_->signal = NULL;
  NUS_command_group_->signal_count = 0;
}
NUS_result nus_command_group_add_semaphores
(NUS_command_group *NUS_command_group_,
 unsigned int wait_count, VkSemaphore *wait,
 unsigned int signal_count, VkSemaphore *signal)
{
  unsigned int i,
    old_wait_count = NUS_command_group_->wait_count,
    old_signal_count = NUS_command_group_->signal_count;
  NUS_command_group_->wait = realloc(NUS_command_group_->wait,
				     sizeof(*NUS_command_group_->wait) *
				     (NUS_command_group_->wait_count +=
				      wait_count));
  NUS_command_group_->signal = realloc(NUS_command_group_->signal,
				     sizeof(*NUS_command_group_->signal) *
				     (NUS_command_group_->signal_count +=
				      signal_count));
  for(i = old_wait_count; i < NUS_command_group_->wait_count; ++i){
    NUS_command_group_->wait[i] = wait[i - old_wait_count];
  }
  for(i = old_signal_count; i < NUS_command_group_->signal_count; ++i){
    NUS_command_group_->signal[i] = signal[i - old_signal_count];
  }
  return NUS_SUCCESS;
}
void nus_command_group_append
(NUS_command_group *p_command_group, VkCommandBuffer command_buffer)
{
  p_command_group->command_buffers = realloc(p_command_group->command_buffers,
					     sizeof(*p_command_group->command_buffers) *
					     ++p_command_group->command_buffer_count);
  p_command_group->command_buffers[p_command_group->command_buffer_count - 1] =
    command_buffer;
}
NUS_result nus_command_group_submit
(NUS_command_group *NUS_command_group_, VkDevice logical_device,
 VkFence commands_finished_fence)
{
  if(NUS_command_group_->command_buffer_count > 0){
    VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkSubmitInfo submit_info = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .pNext = NULL,
      .waitSemaphoreCount = NUS_command_group_->wait_count,
      .pWaitSemaphores = NUS_command_group_->wait,
      .pWaitDstStageMask = &wait_dst_stage_mask,
      .commandBufferCount = NUS_command_group_->command_buffer_count,
      .pCommandBuffers = NUS_command_group_->command_buffers,
      .signalSemaphoreCount = NUS_command_group_->signal_count,
      .pSignalSemaphores = NUS_command_group_->signal
    };
    if(vkQueueSubmit(NUS_command_group_->queue, 1,
		     &submit_info, commands_finished_fence) != VK_SUCCESS){
      printf("ERROR::failed to submit queue\n");
      return NUS_FAILURE;
    }
    if(nus_command_group_reset_buffers(NUS_command_group_) != NUS_SUCCESS){
      printf("ERROR::failed to reset command queue command buffers\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
static NUS_result nus_command_group_reset_buffers
(NUS_command_group *NUS_command_group_)
{
  NUS_command_group_->command_buffers = NULL;
  NUS_command_group_->wait = NULL;
  NUS_command_group_->signal = NULL;
  NUS_command_group_->command_buffer_count = 0;
  NUS_command_group_->wait_count = 0;
  NUS_command_group_->signal_count = 0;
  return NUS_SUCCESS;
}
