#ifndef NUS_POSE_SKELETON_H
#define NUS_POSE_SKELETON_H

#include "../math/NUS_matrix.h"
#include "NUS_joint_pose.h"

struct NUS_skeleton;

typedef struct NUS_pose_skeleton{
  struct NUS_skeleton *p_skeleton;
  NUS_matrix *transformations;
  NUS_joint_pose *pose_joints;
} NUS_pose_skeleton;

NUS_pose_skeleton nus_pose_skeleton_build(struct NUS_skeleton */*more*/);
void nus_pose_skeleton_free(NUS_pose_skeleton *);
NUS_pose_skeleton nus_pose_skeleton_interpolate
(NUS_pose_skeleton, NUS_pose_skeleton, double);

#endif /* NUS_POSE_SKELETON_H */
