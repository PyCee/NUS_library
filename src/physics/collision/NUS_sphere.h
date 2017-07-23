#ifndef NUS_SPHERE_H
#define NUS_SPHERE_H

#include "../../math/NUS_vector.h"
#include "NUS_collision_type.h"

typedef struct NUS_sphere{
  NUS_collision_type type;
  NUS_vector center;
  float radius;
} NUS_sphere;

NUS_sphere nus_sphere_build(NUS_vector, float);

#endif /* NUS_SPHERE_H */
