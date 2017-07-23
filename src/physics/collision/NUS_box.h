#ifndef NUS_BOX_H
#define NUS_BOX_H

#include "../../math/NUS_vector.h"
#include "NUS_collision_type.h"

typedef struct NUS_box{
  NUS_collision_type type;
  NUS_vector position;
} NUS_box;

NUS_box nus_box_build(NUS_vector);

#endif /* NUS_BOX_H */
