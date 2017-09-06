#ifndef NUS_SKELETON_H
#define NUS_SKELETON_H

#include "../math/NUS_matrix.h"
#include <stdint.h>

#define NUS_SKELETON_JOINT_NAME_SIZE 20

struct NUS_keyframe;

typedef struct NUS_skeleton_joint{
  NUS_matrix inv_bind_pose;
  /* Model-Space --> Joint-Space */
  
  int16_t parent_index;
  char name[NUS_SKELETON_JOINT_NAME_SIZE];
} NUS_skeleton_joint;

typedef struct NUS_skeleton{
  uint32_t joint_count;
  NUS_skeleton_joint *joints;
} NUS_skeleton;

NUS_skeleton nus_skeleton_build(NUS_skeleton *);
void nus_skeleton_free(NUS_skeleton *);

typedef struct NUS_skeleton_pose{
  NUS_skeleton *p_skeleton;
  NUS_matrix *skinning_matrices;
} NUS_skeleton_pose;
NUS_skeleton_pose nus_skeleton_pose_build(NUS_skeleton *);
void nus_skeleton_pose_free(NUS_skeleton_pose *);
void nus_skeleton_pose_update(NUS_skeleton_pose *, struct NUS_keyframe);

#endif /* NUS_SKELETON_H */
