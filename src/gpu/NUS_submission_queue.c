#include "NUS_submission_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "../NUS_log.h"
#include "../NUS_vulkan.h"

static void nus_submission_queue_reset(NUS_submission_queue *);

NUS_result nus_submission_queue_build
(VkDevice device, unsigned int family_index, unsigned int queue_index,
 NUS_submission_queue *p_submission_queue)
{
  VkFenceCreateInfo fence_info = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0
  };
  if(vkCreateFence(device, &fence_info, NULL, &p_submission_queue->queue_finished) !=
     VK_SUCCESS){
    NUS_LOG_ERROR("failed to create submission_queue finished fence\n");
    return NUS_FAILURE;
  }
  
  vkGetDeviceQueue(device, family_index, queue_index, &p_submission_queue->queue);

  p_submission_queue->info = (VkSubmitInfo){
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = 0,
    .pWaitSemaphores = p_submission_queue->wait_sem,
    .pWaitDstStageMask = p_submission_queue->wait_dst_mask,
    .commandBufferCount = 0,
    .pCommandBuffers = p_submission_queue->buffer,
    .signalSemaphoreCount = 0,
    .pSignalSemaphores = p_submission_queue->signal_sem
  };
  
  return NUS_SUCCESS;
}
void nus_submission_queue_free
(NUS_submission_queue *p_submission_queue, VkDevice logical_device)
{
  vkQueueWaitIdle(p_submission_queue->queue);
  vkDestroyFence(logical_device, p_submission_queue->queue_finished, NULL);
}
NUS_result nus_submission_queue_add_wait_semaphore
(NUS_submission_queue *p_submission_queue, VkSemaphore wait_semaphore,
 VkPipelineStageFlags flags)
{
  int semaphore_index = p_submission_queue->info.waitSemaphoreCount;
  if(++p_submission_queue->info.waitSemaphoreCount >=
     NUS_SUBMISSION_QUEUE_WAIT_COUNT){
    NUS_LOG_ERROR("failed to add wait semaphore to submission_queue\n");
    return NUS_FAILURE;
  }
  p_submission_queue->wait_sem[semaphore_index] = wait_semaphore;
  p_submission_queue->wait_dst_mask[semaphore_index] = flags;
  return NUS_SUCCESS;
}
NUS_result nus_submission_queue_add_signal_semaphore
(NUS_submission_queue *p_submission_queue, VkSemaphore signal_semaphore)
{
  int semaphore_index = p_submission_queue->info.signalSemaphoreCount;
  if(++p_submission_queue->info.signalSemaphoreCount >=
     NUS_SUBMISSION_QUEUE_SIGNAL_COUNT){
    NUS_LOG_ERROR("failed to add signal semaphore to submission_queue\n");
    return NUS_FAILURE;
  }
  p_submission_queue->signal_sem[semaphore_index] = signal_semaphore;
  return NUS_SUCCESS;
}
NUS_result nus_submission_queue_add_buffer
(NUS_submission_queue *p_submission_queue, VkCommandBuffer command_buffer)
{
  int buffer_index = p_submission_queue->info.commandBufferCount;
  if(++p_submission_queue->info.commandBufferCount >=
     NUS_SUBMISSION_QUEUE_BUFFER_COUNT){
    NUS_LOG_ERROR("failed to add command buffer to submission_queue\n");
    return NUS_FAILURE;
  }
  p_submission_queue->buffer[buffer_index] = command_buffer;
  return NUS_SUCCESS;
}
NUS_result nus_submission_queue_submit
(NUS_submission_queue *p_submission_queue, VkDevice logical_device)
{
  if(p_submission_queue->info.commandBufferCount > 0){
    // If there are command buffers to submit
    vkQueueWaitIdle(p_submission_queue->queue);
    vkResetFences(logical_device, 1, &p_submission_queue->queue_finished);
    
    if(vkQueueSubmit(p_submission_queue->queue, 1, &p_submission_queue->info,
		     p_submission_queue->queue_finished) != VK_SUCCESS){
      NUS_LOG_ERROR("failed queue submission_queue\n");
      return NUS_FAILURE;
    }
    nus_submission_queue_reset(p_submission_queue);
  }
  return NUS_SUCCESS;
}
NUS_result nus_submission_queue_wait
(NUS_submission_queue *p_submission_queue, VkDevice logical_device)
{
  if(vkWaitForFences(logical_device, 1, &p_submission_queue->queue_finished,
		     VK_TRUE, NUS_SUBMISSION_QUEUE_WAIT_TIMEOUT) == VK_TIMEOUT){
    // If the previous submission_queue is taking an extreme ammount of time
    NUS_LOG_ERROR("submission_queue finished fence timeout\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static void nus_submission_queue_reset(NUS_submission_queue *p_submission_queue)
{
  p_submission_queue->info.waitSemaphoreCount = 0;
  p_submission_queue->info.commandBufferCount = 0;
  p_submission_queue->info.signalSemaphoreCount = 0;
}
