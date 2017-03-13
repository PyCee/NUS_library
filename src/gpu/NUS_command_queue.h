#ifndef NUS_COMMAND_QUEUE_H
#define NUS_COMMAND_QUEUE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

typedef struct NUS_command_queue{
  VkQueue queue;
  VkCommandBuffer *command_buffers;
  VkSemaphore *wait,
    *signal;
  unsigned int command_buffer_count,
    wait_count,
    signal_count;
} NUS_command_queue;

NUS_result nus_command_queue_build
(VkDevice, unsigned int, unsigned int, NUS_command_queue *);
void nus_command_queue_free(NUS_command_queue *);
NUS_result nus_command_queue_add_semaphores
(NUS_command_queue *, unsigned int, VkSemaphore *,
 unsigned int, VkSemaphore *);
NUS_result nus_command_queue_add_buffer
(NUS_command_queue *, VkDevice, VkCommandPool, VkCommandBuffer *);
NUS_result nus_command_queue_submit
(NUS_command_queue *, VkDevice, VkCommandPool);

#endif /* NUS_COMMAND_QUEUE_H */
