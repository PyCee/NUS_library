#include "NUS_memory_map.h"
#include "../NUS_log.h"
#include <stdio.h>
#include <string.h>

NUS_result nus_memory_map_build
(size_t memory_size, VkBufferUsageFlags usage,
 VkMemoryPropertyFlags memory_property_flags, NUS_memory_map *p_memory_map)
{
  p_memory_map->binding = nus_get_binding();
  p_memory_map->size = memory_size;
  
  if(memory_property_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT){
    memory_property_flags ^= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    NUS_LOG_WARNING("using memory map property VK_MEMORY_PROPERTY_HOST_COHERENT_BIT\n");
  }
  // vkFlushMappedMemory and vkInvalidateMappedMemoryRanges are not needed if the
  // map is created with the memory property VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  // but it is assumed disabled for minor preformance reasons. Attempting to
  // create a map with it currently only triggers a warning, but that may change
  // in the future
  
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
  if(vkCreateBuffer(nus_get_bound_gpu()->logical_device, &buffer_create_info, NULL,
		    &p_memory_map->buffer) != VK_SUCCESS){
    printf("ERROR::failed to create model buffer\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements buffer_memory_req;
  vkGetBufferMemoryRequirements(nus_get_bound_gpu()->logical_device,
				p_memory_map->buffer, &buffer_memory_req);
  
  VkMemoryAllocateInfo memory_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = buffer_memory_req.size,
    .memoryTypeIndex =
    nus_gpu_memory_type_index(*nus_get_bound_gpu(), buffer_memory_req,
			      memory_property_flags)
  };
  if(vkAllocateMemory(nus_get_bound_gpu()->logical_device, &memory_allocate_info,
		      NULL, &p_memory_map->device_memory) != VK_SUCCESS){
    printf("ERROR::failed to allocate memory for model\n");
    return NUS_FAILURE;
  }
  
  if(vkBindBufferMemory(nus_get_bound_gpu()->logical_device, p_memory_map->buffer,
		        p_memory_map->device_memory, 0) != VK_SUCCESS){
    printf("ERROR::failed to bind buffer memory\n");
  }
  return NUS_SUCCESS;
}
void nus_memory_map_free(NUS_memory_map *p_memory_map)
{
  nus_bind_binding(&p_memory_map->binding);
  
  if(p_memory_map->buffer != VK_NULL_HANDLE){
    vkDestroyBuffer(nus_get_bound_device(), p_memory_map->buffer, NULL);
    p_memory_map->buffer = VK_NULL_HANDLE;
  }
  if(p_memory_map->device_memory != VK_NULL_HANDLE){
    vkFreeMemory(nus_get_bound_device(), p_memory_map->device_memory, NULL);
    p_memory_map->device_memory = VK_NULL_HANDLE;
  }
  
  nus_unbind_binding(&p_memory_map->binding);
}
NUS_result nus_memory_map_flush
(NUS_memory_map memory_map, void *p_data, size_t data_size)
{
  void *p_map;
  VkMappedMemoryRange memory_range;

  nus_bind_binding(&memory_map.binding);
  
  // Map gpu memory to the cpu
  if(vkMapMemory(nus_get_bound_device(), memory_map.device_memory, 0,
		 memory_map.size, 0, &p_map) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to map buffer memory\n");
    return NUS_FAILURE;
  }
  
  // Copy data to cpu-mapped memory
  memcpy(p_map, p_data, data_size);
  
  memory_range = (VkMappedMemoryRange){
    .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext = NULL,
    .memory = memory_map.device_memory,
    .offset = 0,
    .size = VK_WHOLE_SIZE
  };
  // Makes sure the memory is mapped to the gpu
  vkFlushMappedMemoryRanges(nus_get_bound_device(), 1, &memory_range);
  
  // Un-map the cpu-side memory
  vkUnmapMemory(nus_get_bound_device(), memory_map.device_memory);
  
  nus_unbind_binding(&memory_map.binding);
  return NUS_SUCCESS;
}
