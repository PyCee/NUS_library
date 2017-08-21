#ifndef NUS_SKELETON_H
#define NUS_SKELETON_H

#include "../math/NUS_matrix.h"

#define NUS_SKELETON_JOINT_NAME_SIZE 20

typedef struct NUS_skeleton_joint{
  NUS_matrix inv_bind_pose;
  int parent_index;
  char name[NUS_SKELETON_JOINT_NAME_SIZE];
} NUS_skeleton_joint;

typedef struct NUS_skeleton{
  NUS_skeleton_joint *joints;
  int joint_count;
} NUS_skeleton;

NUS_skeleton nus_skeleton_build(int /* ... */);
void nus_skeleton_free(NUS_skeleton *);

typedef struct NUS_skeleton_pose{
  NUS_skeleton *p_skeleton;
  NUS_matrix *transformations;
} NUS_skeleton_pose;
void nus_skeleton_pose_build(NUS_skeleton *, NUS_skeleton_pose );

#endif /* NUS_SKELETON_H */
