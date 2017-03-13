#include "NUS_command_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "../NUS_vulkan.h"

static NUS_result nus_command_queue_reset_buffers
(NUS_command_queue *, VkDevice, VkCommandPool);

NUS_result nus_command_queue_build
(VkDevice device, unsigned int family_index, unsigned int queue_index,
 NUS_command_queue *NUS_command_queue_)
{
  vkGetDeviceQueue(device, family_index, queue_index, &NUS_command_queue_->queue);
  NUS_command_queue_->command_buffers = NULL;
  NUS_command_queue_->wait = NULL;
  NUS_command_queue_->signal = NULL;
  NUS_command_queue_->command_buffer_count = 0;
  NUS_command_queue_->wait_count = 0;
  NUS_command_queue_->signal_count = 0;
  return NUS_SUCCESS;
}
void nus_command_queue_free(NUS_command_queue *NUS_command_queue_)
{
  if(NUS_command_queue_->command_buffers){
    free(NUS_command_queue_->command_buffers);
    NUS_command_queue_->command_buffers = NULL;
  }
  if(NUS_command_queue_->wait){
    free(NUS_command_queue_->wait);
    NUS_command_queue_->wait = NULL;
  }
  if(NUS_command_queue_->signal){
    free(NUS_command_queue_->signal);
    NUS_command_queue_->signal = NULL;
  }
}
NUS_result nus_command_queue_add_semaphores
(NUS_command_queue *NUS_command_queue_,
 unsigned int wait_count, VkSemaphore *wait,
 unsigned int signal_count, VkSemaphore *signal)
{
  unsigned int i,
    old_wait_count = NUS_command_queue_->wait_count,
    old_signal_count = NUS_command_queue_->signal_count;
  NUS_command_queue_->wait = realloc(NUS_command_queue_->wait,
				     sizeof(*NUS_command_queue_->wait) *
				     (NUS_command_queue_->wait_count +=
				      wait_count));
  NUS_command_queue_->signal = realloc(NUS_command_queue_->signal,
				     sizeof(*NUS_command_queue_->signal) *
				     (NUS_command_queue_->signal_count +=
				      signal_count));
  for(i = old_wait_count; i < NUS_command_queue_->wait_count; ++i){
    NUS_command_queue_->wait[i] = wait[i - old_wait_count];
  }
  for(i = old_signal_count; i < NUS_command_queue_->signal_count; ++i){
    NUS_command_queue_->signal[i] = signal[i - old_signal_count];
  }
  return NUS_SUCCESS;
}
NUS_result nus_command_queue_add_buffer
(NUS_command_queue *NUS_command_queue_, VkDevice logical_device,
 VkCommandPool command_pool, VkCommandBuffer *command_buffer)
{
  VkCommandBufferAllocateInfo command_buffer_allocate_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    NULL,
    command_pool,
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    1
  };
  NUS_command_queue_->command_buffers =
    realloc(NUS_command_queue_->command_buffers,
	    sizeof(NUS_command_queue_->command_buffers) *
	    ++NUS_command_queue_->command_buffer_count);
  if(vkAllocateCommandBuffers(logical_device, &command_buffer_allocate_info,
			      NUS_command_queue_->command_buffers +
			      NUS_command_queue_->command_buffer_count - 1) !=
     VK_SUCCESS){
    printf("ERROR::failed to allocate command buffers\n");
    return NUS_FAILURE;
  }
  *command_buffer = *(NUS_command_queue_->command_buffers +
		      NUS_command_queue_->command_buffer_count - 1);
  return NUS_SUCCESS;
}
NUS_result nus_command_queue_submit
(NUS_command_queue *NUS_command_queue_, VkDevice logical_device,
 VkCommandPool command_pool)
{
  VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = NUS_command_queue_->wait_count,
    .pWaitSemaphores = NUS_command_queue_->wait,
    .pWaitDstStageMask = &wait_dst_stage_mask,
    .commandBufferCount = NUS_command_queue_->command_buffer_count,
    .pCommandBuffers = NUS_command_queue_->command_buffers,
    .signalSemaphoreCount = NUS_command_queue_->signal_count,
    .pSignalSemaphores = NUS_command_queue_->signal
  };
  if(vkQueueSubmit(NUS_command_queue_->queue, 1,
		     &submit_info, VK_NULL_HANDLE) != VK_SUCCESS){
    printf("ERROR::failed to submit queue\n");
    return NUS_FAILURE;
  }
  if(nus_command_queue_reset_buffers(NUS_command_queue_, logical_device,
				     command_pool) != NUS_SUCCESS){
    printf("ERROR::failed to reset command queue command buffers\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_command_queue_reset_buffers
(NUS_command_queue *NUS_command_queue_, VkDevice logical_device,
 VkCommandPool command_pool)
{
  //TODO: submit fences with commands so I know when it is safe to free command butters
  // TODO priority to the TODO above
  /*
  vkFreeCommandBuffers(logical_device, command_pool,
		       NUS_command_queue_->command_buffer_count,
		       NUS_command_queue_->command_buffers);
  
  free(NUS_command_queue_->command_buffers);
  */
  NUS_command_queue_->command_buffers = NULL;
  NUS_command_queue_->command_buffer_count = 0;
  
  
  NUS_command_queue_->wait = NULL;
  NUS_command_queue_->wait_count = 0;
  NUS_command_queue_->signal = NULL;
  NUS_command_queue_->signal_count = 0;
  
  return NUS_SUCCESS;
}
