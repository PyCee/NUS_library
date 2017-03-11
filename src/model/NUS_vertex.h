#ifndef NUS_VERTEX_H
#define NUS_VERTEX_H

#include "../math/NUS_vector.h"

typedef struct NUS_vertex{
  double attributes[3/*position*/ + 3/*normal*/ + 2/*texture coords - uv*/];
} NUS_vertex;

NUS_vertex nus_vertex_build(NUS_vector, NUS_vector, NUS_vector);
void nus_vertex_free(NUS_vertex *);
NUS_vector nus_vertex_position(NUS_vertex);
NUS_vector nus_vertex_normal(NUS_vertex);
NUS_vector nus_vertex_uv(NUS_vertex);

#endif /* NUS_VERTEX_H */
