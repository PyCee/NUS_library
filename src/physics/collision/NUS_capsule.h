#ifndef NUS_CAPSULE_H
#define NUS_CAPSULE_H

#include "../../math/NUS_vector.h"
#include "NUS_collision_type.h"

typedef struct NUS_capsule{
  NUS_collision_type type;
  NUS_vector position_0, position_1;
  float radius;
} NUS_capsule;

NUS_capsule nus_capsule_build(NUS_vector, NUS_vector, float);

#endif /* NUS_CAPSULE_H */
