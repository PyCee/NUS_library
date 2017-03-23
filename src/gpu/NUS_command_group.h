#ifndef NUS_COMMAND_GROUP_H
#define NUS_COMMAND_GROUP_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

struct NUS_queue_info;

typedef struct NUS_command_group{
  VkQueue queue;
  VkCommandBuffer *command_buffers;
  VkSemaphore *wait,
    *signal;
  unsigned int command_buffer_count,
    wait_count,
    signal_count;
} NUS_command_group;

NUS_result nus_command_group_build
(VkDevice, unsigned int, unsigned int, NUS_command_group *);
void nus_command_group_free(NUS_command_group *);
NUS_result nus_command_group_add_semaphores
(NUS_command_group *, unsigned int, VkSemaphore *,
 unsigned int, VkSemaphore *);
void nus_command_group_append(NUS_command_group *, VkCommandBuffer);
NUS_result nus_command_group_submit
(NUS_command_group *, VkDevice, VkFence);

#endif /* NUS_COMMAND_GROUP_H */
