#ifndef NUS_MEMORY_MAP_H
#define NUS_MEMORY_MAP_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

struct NUS_queue_info;

typedef struct NUS_memory_map{
  VkBuffer buffer;
  VkDeviceMemory device_memory;
  size_t size;
} NUS_memory_map;

NUS_result nus_memory_map_build
(struct NUS_queue_info, size_t, unsigned int, unsigned int, NUS_memory_map *);
void nus_memory_map_free(NUS_memory_map *, struct NUS_queue_info);
NUS_result nus_memory_map_flush(NUS_memory_map, struct NUS_queue_info, void *);

#endif /* NUS_MEMORY_MAP_H */
