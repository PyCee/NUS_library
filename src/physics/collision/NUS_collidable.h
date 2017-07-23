#ifndef NUS_COLLIDABLE_H
#define NUS_COLLIDABLE_H

#include "NUS_aabb.h"

typedef struct NUS_collidable{
  NUS_aabb bounding_box;
} NUS_collidable;

NUS_collidable nus_collidable_build(NUS_aabb);

#endif /* NUS_COLLIDABLE_H */
