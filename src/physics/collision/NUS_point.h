#ifndef NUS_POINT_H
#define NUS_POINT_H

#include "../../math/NUS_vector.h"
#include "NUS_collision_type.h"

typedef struct NUS_point{
  NUS_collision_type type;
  NUS_vector position;
} NUS_point;

NUS_point nus_point_build(NUS_vector);

#endif /* NUS_POINT_H */
