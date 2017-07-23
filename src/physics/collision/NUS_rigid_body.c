#include "NUS_rigid_body.h"

NUS_rigid_body nus_rigid_body_build_point(NUS_vector position)
{
  return (NUS_rigid_body){.point = nus_point_build(position)};
}
NUS_rigid_body nus_rigid_body_build_sphere(NUS_vector center, float radius)
{
  return (NUS_rigid_body){.sphere = nus_sphere_build(center, radius)};
}
/*
NUS_rigid_body nus_rigid_body_build_box
(NUS_vector center, float width, float height, float depth)
{
  return (NUS_rigid_body){.box = nus_box_build(center, width, height, depth)};
}
*/
NUS_rigid_body nus_rigid_body_build_capsule
(NUS_vector position_0, NUS_vector position_1, float radius)
{
  return (NUS_rigid_body){
    .capsule = nus_capsule_build(position_0, position_1, radius)
      };
}
