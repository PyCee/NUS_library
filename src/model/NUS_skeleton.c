#include "NUS_skeleton.h"
#include <stdlib.h>
#include <string.h>
#include "NUS_keyframe.h"
#include "../NUS_log.h"
#include "../math/NUS_axes.h"

NUS_skeleton nus_skeleton_build(NUS_skeleton *p_skeleton)
{
  NUS_skeleton skeleton;
  size_t joint_data_size;
  skeleton.joint_count = p_skeleton->joint_count;
  joint_data_size = sizeof(*skeleton.joints) * skeleton.joint_count;
  skeleton.joints = malloc(joint_data_size);
  memcpy(skeleton.joints, p_skeleton->joints, joint_data_size);
  
  return skeleton;
}
void nus_skeleton_free(NUS_skeleton *p_skeleton)
{
  if(p_skeleton->joint_count){
    free(p_skeleton->joints);
    p_skeleton->joints = NULL;
  }
  p_skeleton->joint_count = 0;
}

NUS_skeleton_pose nus_skeleton_pose_build
(NUS_skeleton *p_skeleton)
{
  NUS_skeleton_pose pose;
  pose.p_skeleton = p_skeleton;
  pose.skinning_matrices =
    malloc(sizeof(*pose.skinning_matrices) * p_skeleton->joint_count);
  memset(pose.skinning_matrices, 0,
	 sizeof(*pose.skinning_matrices) * p_skeleton->joint_count);
  
  return pose;
}
void nus_skeleton_pose_free(NUS_skeleton_pose *p_pose)
{
  free(p_pose->skinning_matrices);
}

void nus_skeleton_pose_update(NUS_skeleton_pose *p_pose, NUS_keyframe keyframe)
{// TODO Check results of the function
  NUS_matrix tmp_transformation;
  NUS_axes tmp_rotation;
  int i;
  int16_t curr_joint_i;
  
  for(i = 0; i < p_pose->p_skeleton->joint_count; ++i){
    // For each transformation
    
    p_pose->skinning_matrices[i] = p_pose->p_skeleton->joints[i].inv_bind_pose;
    curr_joint_i = i;
    do{
      tmp_rotation = nus_axes_build_qua(keyframe.joints[curr_joint_i].rotation);
      tmp_transformation =
	nus_matrix_build_transformation(keyframe.joints[curr_joint_i].translation,
					keyframe.joints[curr_joint_i].scale,
					tmp_rotation);
      p_pose->skinning_matrices[i] =
	nus_matrix_multiply(tmp_transformation, p_pose->skinning_matrices[i]);
      curr_joint_i = p_pose->p_skeleton->joints[curr_joint_i].parent_index;
      // While current joint has a parent
    } while(curr_joint_i != -1);
    
    //p_pose->skinning_matrices[i] = nus_matrix_transpose(p_pose->skinning_matrices[i]);
    //p_pose->skinning_matrices[i] = nus_matrix_identity();
    
    NUS_LOG("inv should be:\n");
    nus_matrix_print(nus_matrix_inverse(p_pose->skinning_matrices[i]));
    NUS_LOG("is:\n");
    nus_matrix_print(nus_matrix_inverse(p_pose->p_skeleton->joints[i].inv_bind_pose));

    NUS_LOG("mult is:\n");
    nus_matrix_print(nus_matrix_multiply(p_pose->p_skeleton->joints[i].inv_bind_pose, p_pose->skinning_matrices[i] ));
  }
}
