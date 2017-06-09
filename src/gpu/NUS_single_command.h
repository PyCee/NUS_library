#ifndef NUS_SINGLE_COMMAND_BUFFER_H
#define NUS_SINGLE_COMMAND_BUFFER_H

#include "NUS_queue_info.h"

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

NUS_result nus_single_command_begin(NUS_queue_info, VkCommandBuffer *);
NUS_result nus_single_command_end(NUS_queue_info);

#endif /* NUS_SINGLE_COMMAND_BUFFER_H */
