#include "NUS_pose_skeleton.h"
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../math/NUS_axes.h"

static void nus_pose_skeleton_build_transformations(NUS_pose_skeleton *);
static void nus_pose_skeleton_build_joint_transformation
(NUS_pose_skeleton *, unsigned int, char *);

NUS_pose_skeleton nus_pose_skeleton_build(NUS_skeleton *p_skeleton/*more*/)
{
  NUS_pose_skeleton pose_skeleton;
  unsigned int i;
  
  pose_skeleton.p_skeleton = p_skeleton;
  pose_skeleton.joints = malloc(sizeof(*pose_skeleton.joints) *
				p_skeleton->joint_count);
  pose_skeleton.global_transformations =
    malloc(sizeof(*pose_skeleton.global_transformations) *
	   p_skeleton->joint_count);

  for(i = 0; i < p_skeleton->joint_count; ++i){
    // build pose_joints from parameters
    //pose_skeleton.joints[i] = nus_pose_joint_build(joint_rot, joint_trans);
  }
  nus_pose_skeleton_build_transformations(&pose_skeleton);
  return pose_skeleton;
}
void nus_pose_skeleton_free(NUS_pose_skeleton *p_pose_skeleton)
{
  unsigned int i;
  for(i = 0; i < p_pose_skeleton->p_skeleton->joint_count; ++i){
    nus_pose_joint_free(p_pose_skeleton->joints + i);
  }
  free(p_pose_skeleton->joints);
  p_pose_skeleton->joints = NULL;
  free(p_pose_skeleton->global_transformations);
  p_pose_skeleton->global_transformations = NULL;
}
NUS_pose_skeleton nus_pose_skeleton_interpolate
(NUS_pose_skeleton pose_skeleton_0, NUS_pose_skeleton pose_skeleton_1, double t)
{
  unsigned int i;
  assert(pose_skeleton_0.p_skeleton == pose_skeleton_1.p_skeleton);
  for(i = 0; i < pose_skeleton_0.p_skeleton->joint_count; ++i){
    // For each pose_joint
    pose_skeleton_0.joints[i] = nus_pose_joint_interpolate(pose_skeleton_0.joints[i],
							   pose_skeleton_1.joints[i],
							   t);
  }
  nus_pose_skeleton_build_transformations(&pose_skeleton_0);
  return pose_skeleton_0;
}

static void nus_pose_skeleton_build_transformations
(NUS_pose_skeleton *p_pose_skeleton)
{
  unsigned int i;
  char calculated[p_pose_skeleton->p_skeleton->joint_count];
  memset(calculated, 0, p_pose_skeleton->p_skeleton->joint_count);
  
  for(i = 0; i < p_pose_skeleton->p_skeleton->joint_count; ++i){
    if(calculated[i] == 0){
      // If joint transformation has not been calculated
      nus_pose_skeleton_build_joint_transformation(p_pose_skeleton, i, calculated);
    }
  }
}
static void nus_pose_skeleton_build_joint_transformation
(NUS_pose_skeleton *p_pose_skeleton, unsigned int joint_index, char *calculated)
{
  NUS_matrix local_transformation, parent_transformation;
  NUS_axes local_rotation;
  NUS_pose_joint *p_pose_joint;
  unsigned int parent_index;
  parent_index = p_pose_skeleton->p_skeleton->joints[joint_index].parent_index;
  p_pose_joint = p_pose_skeleton->joints + joint_index;
  local_rotation = nus_axes_global_rotation(NUS_AXES_DEFAULT_FORWARD,
					    p_pose_joint->rot);
  local_transformation = nus_matrix_transformation(p_pose_joint->trans,
						   local_rotation);
  if(parent_index != UINT_MAX && calculated[parent_index] == 0){
    // If parent_index is valid and 
    //    parent transformation has not yet been calculated
    nus_pose_skeleton_build_joint_transformation(p_pose_skeleton, parent_index,
						 calculated);
  }
  parent_transformation = p_pose_skeleton->global_transformations[parent_index];
  p_pose_skeleton->global_transformations[joint_index] =
    nus_matrix_multiply(local_transformation,
			parent_transformation);
  calculated[joint_index] = 1;
}
