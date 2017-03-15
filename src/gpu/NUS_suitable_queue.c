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
(NUS_suitable_queue info, VkCommandBuffer *command_buffer)
{
  VkCommandBufferAllocateInfo command_buffer_allocate_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    NULL,
    info.p_queue_family->command_pool[info.p_queue_family->active_command_pool_index],
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    1
  };
  info.p_command_group->command_buffers =
    realloc(info.p_command_group->command_buffers,
	    sizeof(info.p_command_group->command_buffers) *
	    ++info.p_command_group->command_buffer_count);
  if(vkAllocateCommandBuffers(info.p_gpu->logical_device,
			      &command_buffer_allocate_info,
			      info.p_command_group->command_buffers +
			      info.p_command_group->command_buffer_count - 1) !=
     VK_SUCCESS){
    printf("ERROR::failed to allocate command buffers\n");
    return NUS_FAILURE;
  }
  *command_buffer = *(info.p_command_group->command_buffers +
		      info.p_command_group->command_buffer_count - 1);
  return NUS_SUCCESS;
}
NUS_result nus_suitable_queue_submit(struct NUS_suitable_queue suitable_queue)
{
  nus_command_group_submit(suitable_queue.p_command_group,
			   suitable_queue.p_gpu->logical_device,
			   suitable_queue.p_queue_family->group_fences
			   [suitable_queue.p_queue_family->active_command_pool_index]
			   [suitable_queue.command_group_index]);
  return NUS_SUCCESS;
}
