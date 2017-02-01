#ifndef NUS_COMMAND_QUEUE_H
#define NUS_COMMAND_QUEUE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

typedef struct NUS_command_queue{
  VkQueue queue;
  
} NUS_command_queue;

NUS_result nus_command_queue_build(NUS_command_queue *);

#endif /* NUS_COMMAND_QUEUE_H */
