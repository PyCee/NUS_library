#ifndef NUS_COLLISION_INFO_H
#define NUS_COLLISION_INFO_H

#include "../../math/NUS_vector.h"

typedef struct NUS_collision_info{
  NUS_vector point_of_collision;
  float penetration;
} NUS_collision_info;

#endif /* NUS_COLLISION_INFO_H */
