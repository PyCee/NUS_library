#include "NUS_octree.h"
#include <stdio.h>
#include <stdlib.h>

static NUS_octree_t **nus_octrees = NULL;
static NUS_octree *nus_octree_handles = NULL,
  nus_octree_count = 0;

NUS_octree nus_octree_build
(NUS_octree parent_handle, double size, NUS_vector position)
{
  nus_octree_handles = realloc(nus_octree_handles, sizeof(*nus_octree_handles) *
			       (nus_octree_count + 1));
  nus_octree_handles[nus_octree_count] = nus_octree_next_handle();
  nus_octrees = realloc(nus_octrees, sizeof(*nus_octrees) * ++nus_octree_count);
  nus_octrees[nus_octree_count - 1] = malloc(sizeof(**nus_octrees));
  *nus_octrees[nus_octree_count - 1] = (NUS_octree_t){
    .parent_handle = parent_handle,
    .children_handle = {0, 0, 0, 0, 0, 0, 0, 0},
    .size = size,
    .position = position,
    .valid_children = 0
  };
  return nus_octree_handles[nus_octree_count - 1];
}
void nus_octree_create_children(NUS_octree NUS_octree_handle)
{
  if(nus_octree_get(NUS_octree_handle).valid_children == 0){
    //make children
  }
}
NUS_octree_t nus_octree_get(NUS_octree NUS_octree_handle)
{
  NUS_octree i;
  for(i = 0; i < nus_octree_count; ++i){
    if(NUS_octree_handle == nus_octrees[i]->handle){
      return *nus_octrees[i];
    }
  }
  printf("ERROR::Invalid NUS_octree_handle\n");
  return *nus_octrees[0];
}
NUS_octree nus_octree_next_handle(void)
{
  NUS_octree used[nus_octree_count],
    i;
  for(i = 0; i < nus_octree_count; ++i){
    used[i] = 0;
    if(nus_octree_handles[i] < nus_octree_count){
      used[i] = 1;
      break;
    }
  }
  for(i = 0; i < nus_octree_count; ++i){
    if(0 == used[i]){
      return i;
    }
  }
  return nus_octree_count;
}
