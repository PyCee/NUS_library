#ifndef NUS_MEMORY_PROPERTIES_H
#define NUS_MEMORY_PROPERTIES_H

#include "../NUS_vulkan.h"

struct NUS_gpu;

unsigned int nus_memory_properties_type_index
(struct NUS_gpu, VkMemoryRequirements, unsigned int);

#endif /* NUS_MEMROY_PROPERTIES_H */
