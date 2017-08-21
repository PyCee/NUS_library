#ifndef NUS_JOINT_POSE_H
#define NUS_JOINT_POSE_H

#include "../math/NUS_quaternion.h"
#include "../math/NUS_vector.h"

typedef struct NUS_joint_pose{
  NUS_quaternion rotation;
  NUS_vector translation;
  NUS_vector scale;
} NUS_joint_pose;

NUS_joint_pose nus_joint_pose_interpolate(NUS_joint_pose, NUS_joint_pose, float);

#endif /* NUS_JOINT_POSE_H */
