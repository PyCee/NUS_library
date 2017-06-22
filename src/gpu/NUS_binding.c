#include "NUS_binding.h"
#include "NUS_multi_gpu.h"
#include "../NUS_log.h"
#include <limits.h>

static NUS_binding nus_gpu_binding;

void (*nus_unbind_binding)(NUS_binding *) = nus_bind_binding;

NUS_binding nus_get_binding(void)
{
  return nus_gpu_binding;
}
void nus_bind_binding(NUS_binding *p_binding)
{
  NUS_binding tmp_binding = *p_binding;
  *p_binding = nus_gpu_binding;
  nus_gpu_binding = tmp_binding;
}

void nus_set_queue_requirements(unsigned int queue_requirements)
{
  nus_gpu_binding.queue_requirements = queue_requirements;
}
NUS_result nus_select_gpu(NUS_multi_gpu multi_gpu)
{
  // tmp until the below loop selection is implimented
  nus_gpu_binding.p_gpu = multi_gpu.gpus + 0;

  // TODO impliment gpu selection, not practical with current vulkan version
  /*
    int i;
    for(i = 0; i < multi_gpu.gpu_count; ++i){
      // do
    }
   */

  //TODO: set vulkan device-level function pointers
  nus_bind_device_vulkan_library(nus_get_bound_gpu()->functions);

  if(nus_select_queue_family(*nus_get_bound_gpu()) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to select queue family\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_select_queue_family(NUS_gpu gpu)
{
  // TODO: LOW_PRIORITY: compare lowest queue workload across queue_families
  int i;
  for(i = 0; i < gpu.queue_family_count; ++i){
    if((gpu.queue_families[i].flags & nus_gpu_binding.queue_requirements) ==
       nus_gpu_binding.queue_requirements){
      // If the current queue family satisfies requirements
      nus_gpu_binding.p_queue_family = gpu.queue_families + i;
      if(nus_select_submission_queue(*nus_get_bound_queue_family()) != NUS_SUCCESS){
	NUS_LOG_ERROR("failed to select command group\n");
	return NUS_FAILURE;
      }
      return NUS_SUCCESS;
    }
  }
  NUS_LOG_ERROR("failed to find queue family with queue requirements\n");
  return NUS_FAILURE;
}
NUS_result nus_select_submission_queue(NUS_queue_family queue_family)
{
  unsigned int i,
    least_workload = UINT_MAX;
  for(i = 0; i < queue_family.submission_queue_count; ++i){
    if(queue_family.p_submission_queues[i].info.commandBufferCount < least_workload){
      least_workload = queue_family.p_submission_queues[i].info.commandBufferCount;
      nus_gpu_binding.p_submission_queue = queue_family.p_submission_queues + i;
    }
  }
  return NUS_SUCCESS;
}

NUS_gpu *nus_get_bound_gpu(void)
{
  if(nus_gpu_binding.p_gpu == NULL){
    NUS_LOG_ERROR("attempted to get bound gpu where NULL\n");
  }
  return nus_gpu_binding.p_gpu;
}
NUS_queue_family *nus_get_bound_queue_family(void)
{
  if(nus_gpu_binding.p_queue_family == NULL){
    NUS_LOG_ERROR("attempted to get bound queue_family where NULL\n");
  }
  return nus_gpu_binding.p_queue_family;
}
NUS_submission_queue *nus_get_bound_submission_queue(void)
{
  if(nus_gpu_binding.p_submission_queue == NULL){
    NUS_LOG_ERROR("attempted to get bound submission_queue where NULL\n");
  }
  return nus_gpu_binding.p_submission_queue;
}
VkDevice nus_get_bound_device(void)
{
  return nus_get_bound_gpu()->logical_device;
}
int nus_get_bound_queue_family_index(void)
{
  return nus_get_bound_queue_family()->family_index;
}

NUS_result nus_allocate_command_buffer(VkCommandBuffer *p_command_buffer, char count)
{
  VkCommandBufferAllocateInfo command_buffer_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = nus_get_bound_queue_family()->command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = count
  };
  if(vkAllocateCommandBuffers(nus_get_bound_device(), &command_buffer_allocate_info,
			      p_command_buffer) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to allocate command buffers\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_add_wait_semaphore
(VkSemaphore wait_semaphore, VkPipelineStageFlags flags)
{
  return nus_submission_queue_add_wait_semaphore(nus_get_bound_submission_queue(),
						 wait_semaphore, flags);
}
NUS_result nus_add_signal_semaphore(VkSemaphore signal_semaphore)
{
  return nus_submission_queue_add_signal_semaphore(nus_get_bound_submission_queue(),
						   signal_semaphore);
}
NUS_result nus_add_command_buffer(VkCommandBuffer command_buffer)
{
  return nus_submission_queue_add_buffer(nus_get_bound_submission_queue(),
					 command_buffer);
}
NUS_result nus_submit_queue(void)
{
  return nus_submission_queue_submit(nus_get_bound_submission_queue(),
				     nus_get_bound_device());
}
NUS_result nus_wait_on_queue(void)
{
  return nus_submission_queue_wait(nus_get_bound_submission_queue(),
				   nus_get_bound_device());
}
