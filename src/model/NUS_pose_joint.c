#include "NUS_pose_joint.h"

NUS_pose_joint nus_pose_joint_build(NUS_quaternion rot, NUS_vector trans)
{
  return (NUS_pose_joint){
    .rot = rot,
      .trans = trans
      };
}
void nus_pose_joint_free(NUS_pose_joint *p_pose_joint)
{

}
NUS_pose_joint nus_pose_joint_interpolate
(NUS_pose_joint pose_joint_0, NUS_pose_joint pose_joint_1, double t)
{
  return nus_pose_joint_build(nus_quaternion_lerp(pose_joint_0.rot,
							 pose_joint_1.rot, t),
			      nus_vector_interpolate(pose_joint_0.trans,
						     pose_joint_1.trans, t));
}
