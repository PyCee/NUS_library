#include "NUS_model.h"
#include "../gpu/NUS_suitable_queue.h"
#include <stdlib.h>
#include <string.h>
#include "../NUS_memory.h"
#include "stdio.h"

NUS_result nus_model_build(char const * const model_file, NUS_model *p_model)
{
  unsigned int i;
  /*
  p_model->vertex_count =
  p_model->vertices = malloc(sizeof(*p_model->vertices) * p_model->vertex_count);
  
  
  p_model->index_count = 
  p_model->indices = malloc(sizeof(*p_model->indices) * p_model->index_count)
  */
  for(i = 0; i < p_model->vertex_count; ++i){
    /* position */
    p_model->vertices[i].attributes[0] = 0.0;
    p_model->vertices[i].attributes[1] = 0.0;
    p_model->vertices[i].attributes[2] = 0.0;
    /* normal */
    p_model->vertices[i].attributes[3] = 0.0;
    p_model->vertices[i].attributes[4] = 0.0;
    p_model->vertices[i].attributes[5] = 0.0;
    /* tex_coords */
    p_model->vertices[i].attributes[6] = 0.0;
    p_model->vertices[i].attributes[7] = 0.0;
  }
  /*
    set indices
  */  
  return NUS_SUCCESS;
}
void nus_model_free(NUS_model *p_model)
{
  
}
NUS_result nus_model_buffer
(NUS_suitable_queue queue, NUS_model *p_model)
{
  VkDeviceMemory device_memory;
  size_t vertex_memory_size = sizeof(*p_model->vertices) * p_model->vertex_count;
  VkBufferCreateInfo buffer_create_info = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .size = vertex_memory_size,
    .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL
  };
  if(vkCreateBuffer(queue.p_gpu->logical_device, &buffer_create_info, NULL,
		    &p_model->vertex_buffer) != VK_SUCCESS){
    printf("ERROR::failed to create model buffer\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements buffer_memory_req;
  vkGetBufferMemoryRequirements(queue.p_gpu->logical_device,
				p_model->vertex_buffer, &buffer_memory_req);

  VkPhysicalDeviceMemoryProperties memory_properties;
  vkGetPhysicalDeviceMemoryProperties(queue.p_gpu->physical_device,
				      &memory_properties);
  
  VkMemoryAllocateInfo memory_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = buffer_memory_req.size,
    .memoryTypeIndex = nus_vk_memory_type_index(queue, buffer_memory_req,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
  };
  if(vkAllocateMemory(queue.p_gpu->logical_device, &memory_allocate_info,
		      NULL, &device_memory) != VK_SUCCESS){
    printf("ERROR::failed to allocate memory for model\n");
    return NUS_FAILURE;
  }
  
  if(vkBindBufferMemory(queue.p_gpu->logical_device, p_model->vertex_buffer,
			device_memory, 0) != VK_SUCCESS){
    printf("ERROR::failed to bind vertex buffer memory\n");
  }

  void *vertex_buffer_memory;
  if(vkMapMemory(queue.p_gpu->logical_device, device_memory, 0, vertex_memory_size,
		 0, &vertex_buffer_memory) != VK_SUCCESS){
    printf("ERROR::failed to map vertex buffer memory\n");
    return NUS_FAILURE;
  }

  memcpy(vertex_buffer_memory, p_model->vertices, vertex_memory_size);
  printf("copied memory of size: %zu\n", vertex_memory_size);
  VkMappedMemoryRange memory_range = {
    .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .pNext = NULL,
    .memory = device_memory,
    .offset = 0,
    .size = VK_WHOLE_SIZE
  };
  vkFlushMappedMemoryRanges(queue.p_gpu->logical_device, 1, &memory_range);

  vkUnmapMemory(queue.p_gpu->logical_device, device_memory);
  
  return NUS_SUCCESS;
}
