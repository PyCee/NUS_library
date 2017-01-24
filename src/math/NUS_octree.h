#ifndef NUS_OCTREE_H
#define NUS_OCTREE_H

#include "NUS_vector.h"

typedef unsigned int NUS_octree;

typedef struct NUS_octree_t{
  NUS_octree handle, parent_handle, children_handle[8];
  double size;
  NUS_vector position;
  char valid_children;
} NUS_octree_t;

NUS_octree nus_octree_build(NUS_octree, double, NUS_vector);
void nus_octree_create_children(NUS_octree);
NUS_octree_t nus_octree_get(NUS_octree);
NUS_octree nus_octree_next_handle(void);

#endif /* NUS_OCTREE_H */
