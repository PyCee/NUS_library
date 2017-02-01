#ifndef NUS_COMMAND_QUEUE_H
#define NUS_COMMAND_QUEUE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

typedef struct NUS_command_queue{
  VkQueue queue;
  unsigned int workload;
} NUS_command_queue;

NUS_result nus_command_queue_build
(VkDevice, unsigned int, unsigned int, NUS_command_queue *);
void nus_command_queue_free(NUS_command_queue *);

#endif /* NUS_COMMAND_QUEUE_H */
