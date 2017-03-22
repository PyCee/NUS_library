#include "NUS_memory_map.h"
#include "NUS_suitable_queue.h"
#include <stdio.h>
#include <string.h>

NUS_result nus_memory_map_build
(NUS_suitable_queue queue, size_t memory_size, unsigned int usage,
 NUS_memory_map *p_memory_map)
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
  if(vkCreateBuffer(queue.p_gpu->logical_device, &buffer_create_info, NULL,
		    &p_memory_map->buffer) != VK_SUCCESS){
    printf("ERROR::failed to create model buffer\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements buffer_memory_req;
  vkGetBufferMemoryRequirements(queue.p_gpu->logical_device,
				p_memory_map->buffer, &buffer_memory_req);

  VkPhysicalDeviceMemoryProperties memory_properties;
  vkGetPhysicalDeviceMemoryProperties(queue.p_gpu->physical_device,
				      &memory_properties);
  
  unsigned int memory_type_index;
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(queue.p_gpu->physical_device,
				      &mem_properties);
  for(memory_type_index = 0; memory_type_index < mem_properties.memoryTypeCount;
      ++memory_type_index){
    if((buffer_memory_req.memoryTypeBits & (uint32_t)(1 << memory_type_index)) &&
       (mem_properties.memoryTypes[memory_type_index].propertyFlags &
	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)){
      break;
    }
  }
  
  VkMemoryAllocateInfo memory_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = buffer_memory_req.size,
    .memoryTypeIndex = memory_type_index
  };
  if(vkAllocateMemory(queue.p_gpu->logical_device, &memory_allocate_info,
		      NULL, &p_memory_map->device_memory) != VK_SUCCESS){
    printf("ERROR::failed to allocate memory for model\n");
    return NUS_FAILURE;
  }
  
  if(vkBindBufferMemory(queue.p_gpu->logical_device, p_memory_map->buffer,
		        p_memory_map->device_memory, 0) != VK_SUCCESS){
    printf("ERROR::failed to bind buffer memory\n");
  }
  return NUS_SUCCESS;
}
void nus_memory_map_free(NUS_memory_map *p_memory_map, NUS_suitable_queue queue)
{
  if(p_memory_map->buffer != VK_NULL_HANDLE){
    vkDestroyBuffer(queue.p_gpu->logical_device, p_memory_map->buffer, NULL);
    p_memory_map->buffer = VK_NULL_HANDLE;
  }
  if(p_memory_map->device_memory != VK_NULL_HANDLE){
    vkFreeMemory(queue.p_gpu->logical_device, p_memory_map->device_memory, NULL);
    p_memory_map->device_memory = VK_NULL_HANDLE;
  }
}
NUS_result nus_memory_map_flush
(NUS_memory_map memory_map, NUS_suitable_queue queue, void *p_src)
{
  void *p_map;
  if(vkMapMemory(queue.p_gpu->logical_device, memory_map.device_memory, 0,
		 memory_map.size, 0, &p_map) != VK_SUCCESS){
    printf("ERROR::failed to map buffer memory\n");
    return NUS_FAILURE;
  }
  memcpy(p_map, p_src, memory_map.size);
  VkMappedMemoryRange memory_range = {
    .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext = NULL,
    .memory = memory_map.device_memory,
    .offset = 0,
    .size = VK_WHOLE_SIZE
  };
  vkFlushMappedMemoryRanges(queue.p_gpu->logical_device, 1, &memory_range);
  vkUnmapMemory(queue.p_gpu->logical_device, memory_map.device_memory);
  return NUS_SUCCESS;
}
