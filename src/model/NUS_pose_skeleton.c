#include "NUS_pose_skeleton.h"
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../math/NUS_axes.h"
#include "NUS_skeleton.h"

static void nus_pose_skeleton_update(NUS_pose_skeleton *);
static void nus_pose_skeleton_update_joint
(NUS_pose_skeleton *, unsigned int, NUS_bool *);

NUS_pose_skeleton nus_pose_skeleton_build(NUS_skeleton *p_skeleton/*more*/)
{
  NUS_pose_skeleton pose_skeleton;
  unsigned int i;
  
  pose_skeleton.p_skeleton = p_skeleton;
  pose_skeleton.transformations = malloc(sizeof(*pose_skeleton.transformations) *
					 p_skeleton->joint_count);
  pose_skeleton.pose_joints = malloc(sizeof(*pose_skeleton.pose_joints) *
				     p_skeleton->joint_count);
  for(i = 0; i < p_skeleton->joint_count; ++i){
    // build pose_joints from parameters
    //pose_skeleton.joints[i].rot = 
    //pose_skeleton.joints[i].scale = 
    //pose_skeleton.joints[i].trans = 
  }
  nus_pose_skeleton_update(&pose_skeleton);
  return pose_skeleton;
}
void nus_pose_skeleton_free(NUS_pose_skeleton *p_pose_skeleton)
{
  free(p_pose_skeleton->pose_joints);
  p_pose_skeleton->pose_joints = NULL;
  free(p_pose_skeleton->transformations);
  p_pose_skeleton->transformations = NULL;
}
NUS_pose_skeleton nus_pose_skeleton_interpolate
(NUS_pose_skeleton pose_skeleton_0, NUS_pose_skeleton pose_skeleton_1, double t)
{
  assert(pose_skeleton_0.p_skeleton == pose_skeleton_1.p_skeleton);
  unsigned int i;
  NUS_pose_skeleton pose_skeleton;
  
  for(i = 0; i < pose_skeleton_0.p_skeleton->joint_count; ++i){
    // For each pose_joint
    /*
      assemble joints in what ever buffer nus_pose_skeleton_build takes
    */
  }
  
  /*
    NUS_pose_skeleton pose_skeleton = 
    nus_pose_skeleton_build(pose_skeleton_0.p_skeleton, );
  */
  
  nus_pose_skeleton_update(&pose_skeleton);
  return pose_skeleton;
}

static void nus_pose_skeleton_update
(NUS_pose_skeleton *p_pose_skeleton)
{
  unsigned int i;
  NUS_bool calculated[p_pose_skeleton->p_skeleton->joint_count];
  memset(calculated, 0, p_pose_skeleton->p_skeleton->joint_count);
  
  for(i = 0; i < p_pose_skeleton->p_skeleton->joint_count; ++i){
    if(calculated[i] == 0){
      // If joint transformation has not been calculated
      nus_pose_skeleton_update_joint(p_pose_skeleton, i, calculated);
    }
  }
}
static void nus_pose_skeleton_update_joint
(NUS_pose_skeleton *p_pose_skeleton, unsigned int joint_index, NUS_bool *calculated)
{
  // A helper function for nus_post_skeleton_update
  NUS_matrix local_transformation;
  int parent_index;
  NUS_axes rotation;
  NUS_vector scale,
    translation;
  parent_index = p_pose_skeleton->p_skeleton->joints[joint_index].parent_index;
  
  rotation = nus_axes_build(nus_vector_build(0.0, 0.0, 1.0),
			    nus_vector_build(0.0, 1.0, 0.0),
			    nus_vector_build(1.0, 0.0, 0.0));
  rotation =
    nus_axes_global_rotation(rotation,
			     p_pose_skeleton->pose_joints[joint_index].rotation);
  scale = p_pose_skeleton->pose_joints[joint_index].scale;
  translation = p_pose_skeleton->pose_joints[joint_index].translation;
  
  local_transformation = nus_matrix_transformation(translation,
						   scale,
						   rotation);
  
  if(parent_index != -1 && calculated[parent_index] == 0){
    // If parent_index is valid and parent has not yet been calculated
    nus_pose_skeleton_update_joint(p_pose_skeleton, parent_index, calculated);
  }
  p_pose_skeleton->transformations[joint_index] =
    nus_matrix_multiply(local_transformation,
			p_pose_skeleton->transformations[parent_index]);
  //calculated[joint_index] = 1;
  calculated[joint_index] = 1;
}
