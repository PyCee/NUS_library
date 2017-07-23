#ifndef NUS_AABB_H
#define NUS_AABB_H

#include "../../math/NUS_vector.h"
#include "../../NUS_bool.h"

typedef struct NUS_aabb{
  NUS_vector lowest,
    highest;
} NUS_aabb;

NUS_aabb nus_aabb_build(NUS_vector, NUS_vector);
NUS_bool nus_aabb_collision_test(NUS_aabb, NUS_aabb);

#endif /* NUS_AABB_H */
