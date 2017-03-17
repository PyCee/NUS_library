#ifndef NUS_MEMORY_H
#define NUS_MEMORY_H

#include "NUS_vulkan.h"

struct NUS_suitable_queue;

unsigned int nus_vk_memory_type_index
(struct NUS_suitable_queue, VkMemoryRequirements, unsigned int);

#endif /* NUS_MEMORY_H */
