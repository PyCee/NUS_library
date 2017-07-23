#ifndef NUS_RIGID_BODY_H
#define NUS_RIGID_BODY_H

#include "NUS_collision_info.h"
#include "NUS_collision_type.h"
#include "NUS_point.h"
#include "NUS_sphere.h"
#include "NUS_box.h"
#include "NUS_capsule.h"

typedef union NUS_rigid_body{
  NUS_collision_type type;
  NUS_point point;
  NUS_sphere sphere;
  NUS_box box;
  NUS_capsule capsule;
} NUS_rigid_body;

NUS_rigid_body nus_rigid_body_build_point(NUS_vector);
NUS_rigid_body nus_rigid_body_build_sphere(NUS_vector, float);
NUS_rigid_body nus_rigid_body_build_box(NUS_vector, float, float, float/*orientation*/);
NUS_rigid_body nus_rigid_body_build_capsule(NUS_vector, NUS_vector, float);

NUS_collision_info nus_rigid_body_detect_collisions(NUS_rigid_body, NUS_rigid_body);

#endif /* NUS_RIGID_BODY_H */
