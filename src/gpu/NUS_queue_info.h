#ifndef NUS_SUITABLE_QUEUE_H
#define NUS_SUITABLE_QUEUE_H

#include <limits.h>

#include "../NUS_vulkan.h"
#include "NUS_multi_gpu.h"
#include "NUS_gpu.h"
#include "NUS_queue_family.h"
#include "NUS_command_group.h"

typedef struct NUS_queue_info{
  struct NUS_gpu *p_gpu;
  struct NUS_queue_family *p_queue_family;
  struct NUS_command_group *p_command_group;
  unsigned int gpu_index,
    queue_family_index,
    command_group_index;
} NUS_queue_info;

void nus_queue_info_build(NUS_queue_info *);
NUS_result nus_queue_info_add_buffer
(struct NUS_queue_info, VkCommandBuffer *);
NUS_result nus_queue_info_submit(struct NUS_queue_info);

#endif /* NUS_SUITABLE_QUEUE_H */
