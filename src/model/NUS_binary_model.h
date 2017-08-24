#ifndef NUS_BINARY_MODEL_H
#define NUS_BINARY_MODEL_H

#include <stdlib.h>
#include "NUS_vertex.h"
#include "NUS_skeleton.h"
#include "NUS_animation.h"
#include <stdint.h>

#define NUS_BINARY_MODEL_SIZE_COUNT 6
// Number of size_t at the beginning of NUS_binary_model

typedef uint32_t NUS_indice;

typedef struct NUS_binary_model{
  uint32_t vertex_count,
    index_count,
    texture_width,
    texture_height,
    texture_data_size,
    animation_count;

  NUS_vertex *vertices;
  NUS_indice *indices;
  void *texture_data;
  NUS_skeleton skeleton;
  NUS_animation *animations;
} NUS_binary_model;

void nus_binary_model_free(NUS_binary_model *);

#endif /* NUS_BINARY_MODEL_H */
