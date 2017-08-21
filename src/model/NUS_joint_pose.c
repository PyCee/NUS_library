#include "NUS_joint_pose.h"

NUS_joint_pose nus_joint_pose_interpolate
(NUS_joint_pose joint_pose_0, NUS_joint_pose joint_pose_1, float t)
{
  return (NUS_joint_pose){
    .rotation = nus_quaternion_slerp(joint_pose_0.rotation,
				     joint_pose_1.rotation, t),
    .translation = nus_vector_lerp(joint_pose_0.translation,
				   joint_pose_1.translation, t),
    .scale = nus_vector_lerp(joint_pose_0.scale, joint_pose_1.scale, t)
    };
}
