#ifndef NUS_MODEL_H
#define NUS_MODEL_H

#include "../NUS_result.h"
#include "NUS_vertex.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_memory_map.h"

struct NUS_suitable_queue;

typedef struct NUS_model{
  // do I need to store the vertices? or can I just use the VkBuffer in build?
  NUS_vertex *vertices;
  NUS_memory_map vertex_memory,
    index_memory;
  /*
  VkBuffer vertex_buffer,
    index_buffer;
  VkDeviceMemory vertex_buffer_memory,
    index_buffer_memory;
  */
  // do I need to store index info?
  //   buffer it to the gpu, and no need to keep in model, right?
  unsigned int *indices,
    vertex_count,
    index_count;
  // texture
  // skeleton
} NUS_model;

NUS_result nus_model_build(char const * const, NUS_model *);
void nus_model_free(struct NUS_suitable_queue, NUS_model *);
NUS_result nus_model_buffer(struct NUS_suitable_queue, NUS_model *);

#endif /* NUS_MODEL_H */
