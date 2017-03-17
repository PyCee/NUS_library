#ifndef NUS_MODEL_H
#define NUS_MODEL_H

#include "../NUS_result.h"
#include "NUS_vertex.h"
#include "../NUS_vulkan.h"

struct NUS_suitable_queue;

typedef struct NUS_model{
  // do I need to store the vertices? or can I just use the VkBuffer in build?
  NUS_vertex *vertices;
  VkBuffer vertex_buffer;
  // do I need to store index info?
  //   buffer it to the gpu, and no need to keep in model
  unsigned int *indices,
    vertex_count,
    index_count;
  // texture
  // skeleton
} NUS_model;

NUS_result nus_model_build(char const * const, NUS_model *);
void nus_model_free(NUS_model *);
NUS_result nus_model_buffer(struct NUS_suitable_queue, NUS_model *);

#endif /* NUS_MODEL_H */
