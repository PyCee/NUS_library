#include "NUS_suitable_queue.h"
#include <stdlib.h>
#include <stdio.h>

void nus_suitable_queue_build
(NUS_suitable_queue *p_suitable_queue)
{
  p_suitable_queue->p_gpu = NULL;
  p_suitable_queue->p_queue_family = NULL;
  p_suitable_queue->p_command_group = NULL;
  p_suitable_queue->gpu_index = UINT_MAX;
  p_suitable_queue->queue_family_index = UINT_MAX;
  p_suitable_queue->command_group_index = UINT_MAX;
}
NUS_result nus_suitable_queue_add_buffer
(NUS_suitable_queue info, VkCommandBuffer *p_command_buffer)
{
  return nus_queue_family_allocate_command_buffer(*info.p_queue_family,
						  info.p_gpu->logical_device,
						  p_command_buffer);
}
NUS_result nus_suitable_queue_submit(struct NUS_suitable_queue suitable_queue)
{
  nus_command_group_submit(suitable_queue.p_command_group,
			   suitable_queue.p_gpu->logical_device,
			   VK_NULL_HANDLE);
  return NUS_SUCCESS;
}
