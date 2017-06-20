#ifndef NUS_SINGLE_COMMAND_BUFFER_H
#define NUS_SINGLE_COMMAND_BUFFER_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

NUS_result nus_single_command_begin(VkCommandBuffer *);
NUS_result nus_single_command_end(void);

#endif /* NUS_SINGLE_COMMAND_BUFFER_H */
