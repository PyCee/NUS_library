#ifndef NUS_MODEL_H
#define NUS_MODEL_H

#include "../NUS_result.h"
#include "NUS_vertex.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_memory_map.h"

struct NUS_queue_info;

typedef struct NUS_model{
  NUS_vertex *vertices;
  NUS_memory_map vertex_memory,
    index_memory;
  unsigned int *indices,
    vertex_count,
    index_count;
  // texture
  // skeleton
} NUS_model;

NUS_result nus_model_build(char const * const, NUS_model *);
void nus_model_free(struct NUS_queue_info, NUS_model *);
NUS_result nus_model_buffer(struct NUS_queue_info, NUS_model *);

#endif /* NUS_MODEL_H */
