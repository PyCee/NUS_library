#ifndef _NUS_OCTREE_H_
#define _NUS_OCTREE_H_

typedef struct NUS_octree{
  double size;
  struct NUS_octree *parent, *child;
} NUS_octree;

NUS_octree NUS_build_octree(NUS_octree *);
void NUS_create_octree_children(NUS_octree *);

#endif /* _NUS_OCTREE_H_ */
