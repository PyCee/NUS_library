#include "NUS_queue_info.h"
#include <stdlib.h>
#include <stdio.h>

void nus_queue_info_build
(NUS_queue_info *p_queue_info)
{
  p_queue_info->p_gpu = NULL;
  p_queue_info->p_queue_family = NULL;
  p_queue_info->p_command_group = NULL;
  p_queue_info->gpu_index = UINT_MAX;
  p_queue_info->queue_family_index = UINT_MAX;
  p_queue_info->command_group_index = UINT_MAX;
}
NUS_result nus_queue_info_add_buffer
(NUS_queue_info info, VkCommandBuffer *p_command_buffer)
{
  return nus_queue_family_allocate_command_buffer(*info.p_queue_family,
						  info.p_gpu->logical_device,
						  p_command_buffer);
}
NUS_result nus_queue_info_submit(struct NUS_queue_info queue_info)
{
  nus_command_group_submit(queue_info.p_command_group,
			   queue_info.p_gpu->logical_device,
			   VK_NULL_HANDLE);
  return NUS_SUCCESS;
}
