#ifndef NUS_MEMORY_MAP_H
#define NUS_MEMORY_MAP_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"
#include "NUS_binding.h"

typedef struct NUS_memory_map{
  VkBuffer buffer;
  VkDeviceMemory device_memory;
  size_t size;
  NUS_binding binding;
} NUS_memory_map;

NUS_result nus_memory_map_build
(size_t, unsigned int, unsigned int, NUS_memory_map *);
void nus_memory_map_free(NUS_memory_map *);
NUS_result nus_memory_map_flush(NUS_memory_map, void *);

#endif /* NUS_MEMORY_MAP_H */
