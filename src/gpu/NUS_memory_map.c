#include "NUS_memory_map.h"
#include "NUS_queue_info.h"
#include <stdio.h>
#include <string.h>

NUS_result nus_memory_map_build
(NUS_queue_info queue_info, size_t memory_size, unsigned int usage,
 unsigned int memory_properties, NUS_memory_map *p_memory_map)
{
  p_memory_map->size = memory_size;
  VkBufferCreateInfo buffer_create_info = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .size = memory_size,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL
  };
  if(vkCreateBuffer(queue_info.p_gpu->logical_device, &buffer_create_info, NULL,
		    &p_memory_map->buffer) != VK_SUCCESS){
    printf("ERROR::failed to create model buffer\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements buffer_memory_req;
  vkGetBufferMemoryRequirements(queue_info.p_gpu->logical_device,
				p_memory_map->buffer, &buffer_memory_req);
  
  VkMemoryAllocateInfo memory_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = buffer_memory_req.size,
    .memoryTypeIndex =
    nus_gpu_memory_type_index(*queue_info.p_gpu, buffer_memory_req,
			      memory_properties)
  };
  if(vkAllocateMemory(queue_info.p_gpu->logical_device, &memory_allocate_info,
		      NULL, &p_memory_map->device_memory) != VK_SUCCESS){
    printf("ERROR::failed to allocate memory for model\n");
    return NUS_FAILURE;
  }
  
  if(vkBindBufferMemory(queue_info.p_gpu->logical_device, p_memory_map->buffer,
		        p_memory_map->device_memory, 0) != VK_SUCCESS){
    printf("ERROR::failed to bind buffer memory\n");
  }
  return NUS_SUCCESS;
}
void nus_memory_map_free(NUS_memory_map *p_memory_map, NUS_queue_info queue_info)
{
  if(p_memory_map->buffer != VK_NULL_HANDLE){
    vkDestroyBuffer(queue_info.p_gpu->logical_device, p_memory_map->buffer, NULL);
    p_memory_map->buffer = VK_NULL_HANDLE;
  }
  if(p_memory_map->device_memory != VK_NULL_HANDLE){
    vkFreeMemory(queue_info.p_gpu->logical_device, p_memory_map->device_memory, NULL);
    p_memory_map->device_memory = VK_NULL_HANDLE;
  }
}
NUS_result nus_memory_map_flush
(NUS_memory_map memory_map, NUS_queue_info queue_info, void *p_src)
{
  void *p_map;
  VkMappedMemoryRange memory_range;
  memory_range = (VkMappedMemoryRange){
    .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext = NULL,
    .memory = memory_map.device_memory,
    .offset = 0,
    .size = VK_WHOLE_SIZE
  };
  
  // Map gpu memory to the cpu
  if(vkMapMemory(queue_info.p_gpu->logical_device, memory_map.device_memory, 0,
		 memory_map.size, 0, &p_map) != VK_SUCCESS){
    printf("ERROR::failed to map buffer memory\n");
    return NUS_FAILURE;
  }
  
  // Copy data to cpu-mapped memory
  memcpy(p_map, p_src, memory_map.size);
  
  // Makes sure the memory is mapped to the gpu
  vkFlushMappedMemoryRanges(queue_info.p_gpu->logical_device, 1, &memory_range);
  
  // Un-map the cpu-side memory
  vkUnmapMemory(queue_info.p_gpu->logical_device, memory_map.device_memory);
  return NUS_SUCCESS;
}
