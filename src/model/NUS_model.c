#include "NUS_model.h"
#include "../gpu/NUS_queue_info.h"
#include <stdlib.h>
#include <string.h>
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
    /* set position */
    p_model->vertices[i].attributes[0] = 0.0;
    p_model->vertices[i].attributes[1] = 0.0;
    p_model->vertices[i].attributes[2] = 0.0;
    /* set normal */
    p_model->vertices[i].attributes[3] = 0.0;
    p_model->vertices[i].attributes[4] = 0.0;
    p_model->vertices[i].attributes[5] = 0.0;
    /* set tex_coords */
    p_model->vertices[i].attributes[6] = 0.0;
    p_model->vertices[i].attributes[7] = 0.0;
  }
  /*
    set indices
  */  
  return NUS_SUCCESS;
}
void nus_model_free(NUS_queue_info queue, NUS_model *p_model)
{
  nus_memory_map_free(&p_model->vertex_memory, queue);
  nus_memory_map_free(&p_model->index_memory, queue);
  /*
  if(p_model->vertex_buffer != VK_NULL_HANDLE){
    vkDestroyBuffer(queue.p_gpu->logical_device, p_model->vertex_buffer, NULL);
    p_model->vertex_buffer = VK_NULL_HANDLE;
  }
  if(p_model->vertex_buffer_memory != VK_NULL_HANDLE){
    vkFreeMemory(queue.p_gpu->logical_device, p_model->vertex_buffer_memory, NULL);
    p_model->vertex_buffer_memory = VK_NULL_HANDLE;
  }
  if(p_model->index_buffer != VK_NULL_HANDLE){
    vkDestroyBuffer(queue.p_gpu->logical_device, p_model->index_buffer, NULL);
    p_model->index_buffer = VK_NULL_HANDLE;
  }
  if(p_model->index_buffer_memory != VK_NULL_HANDLE){
    vkFreeMemory(queue.p_gpu->logical_device, p_model->index_buffer_memory, NULL);
    p_model->index_buffer_memory = VK_NULL_HANDLE;
  }*/
}
NUS_result nus_model_buffer
(NUS_queue_info queue, NUS_model *p_model)
{
  size_t vertex_memory_size = sizeof(*p_model->vertices) * p_model->vertex_count;
  size_t index_memory_size = sizeof(*p_model->indices) * p_model->index_count;
  
  if(nus_memory_map_build(queue, vertex_memory_size,
			  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			  &p_model->vertex_memory) != NUS_SUCCESS){
    printf("ERROR::failed to build memory map for vertices\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_build(queue, index_memory_size,
			  VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			  &p_model->index_memory) != NUS_SUCCESS){
    printf("ERROR::failed to build memory map for indices\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(p_model->vertex_memory, queue, p_model->vertices) !=
     NUS_SUCCESS){
    printf("ERROR::failed to flush vertex memory\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(p_model->index_memory, queue, p_model->indices) !=
     NUS_SUCCESS){
    printf("ERROR::failed to flush index memory\n");
    return NUS_FAILURE;
  }
  
  return NUS_SUCCESS;
}
