#ifndef NUS_POSE_JOINT_H
#define NUS_POSE_JOINT_H

#include "../math/NUS_quaternion.h"
#include "../math/NUS_vector.h"

typedef struct NUS_pose_joint{
  NUS_quaternion rot;
  NUS_vector trans;
} NUS_pose_joint;

NUS_pose_joint nus_pose_joint_build(NUS_quaternion, NUS_vector);
void nus_pose_joint_free(NUS_pose_joint *);
NUS_pose_joint nus_pose_joint_interpolate(NUS_pose_joint, NUS_pose_joint, double);

#endif /* NUS_POSE_JOINT_H */
