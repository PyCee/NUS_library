#ifndef NUS_POSE_SKELETON_H
#define NUS_POSE_SKELETON_H

#include "NUS_skeleton.h"
#include "NUS_pose_joint.h"
#include "../math/NUS_matrix.h"

typedef struct NUS_pose_skeleton{
  NUS_skeleton *p_skeleton;
  NUS_pose_joint *joints;
  NUS_matrix *global_transformations;
} NUS_pose_skeleton;

NUS_pose_skeleton nus_pose_skeleton_build(NUS_skeleton */*more*/);
void nus_pose_skeleton_free(NUS_pose_skeleton *);
NUS_pose_skeleton nus_pose_skeleton_interpolate
(NUS_pose_skeleton, NUS_pose_skeleton, double);

#endif /* NUS_POSE_SKELETON_H */
