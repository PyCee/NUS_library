#ifndef NUS_VERTEX_H
#define NUS_VERTEX_H

#include <stdint.h>

typedef struct NUS_vertex{
  float position[3],
    normal[3],
    tex_coords[2],
    bone_weights[4];
  uint32_t bone_indices[4];
} NUS_vertex;

#endif /* NUS_VERTEX_H */
