#include "NUS_model.h"
#include "../gpu/NUS_queue_info.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include <NUSM/NUSM_library.h>

NUS_result nus_model_build(NUS_absolute_path absolute_path, NUS_model *p_model)
{
  nusm_read(absolute_path.path, &p_model->contents);
  
  printf("index buffer count: %d\n", (int)p_model->contents.index_data_size /
	 (int)NUSM_INDEX_BYTE_COUNT);
  printf("vertex buffer count: %d\n", (int)p_model->contents.vertex_data_size/
	 (int)NUSM_VERTEX_BYTE_COUNT);
  return NUS_SUCCESS;
}
void nus_model_free(NUS_queue_info queue, NUS_model *p_model)
{
  nus_memory_map_free(&p_model->vertex_memory, queue);
  nus_memory_map_free(&p_model->index_memory, queue);
}
NUS_result nus_model_buffer
(NUS_queue_info queue, NUS_model *p_model)
{
  if(nus_memory_map_build(queue, p_model->contents.vertex_data_size,
			  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			  &p_model->vertex_memory) != NUS_SUCCESS){
    printf("ERROR::failed to build memory map for vertices\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_build(queue, p_model->contents.index_data_size,
			  VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			  &p_model->index_memory) != NUS_SUCCESS){
    printf("ERROR::failed to build memory map for indices\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(p_model->vertex_memory, queue,
			  p_model->contents.vertex_data) != NUS_SUCCESS){
    printf("ERROR::failed to flush vertex memory\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(p_model->index_memory, queue,
			  p_model->contents.index_data) != NUS_SUCCESS){
    printf("ERROR::failed to flush index memory\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
