#include "skeleton.h"
#include "joint.h"
#include "matrix.h"
#include "quaternion.h"
#include <stdlib.h>
#include <string.h>

static void build_global_transformation_poses(skeleton_pose *, unsigned int);

skeleton_pose gen_skeleton_pose(void)
{
	skeleton_pose pose;
	pose.m_skeleton = 0;
	pose.m_local_poses = 0;
	pose.m_global_poses = 0;
	return pose;
}
void rebuild_skeleton_pose_global_poses(sleleton_pose *pose)
{
	unsigned int i = 0;
	skeleton_pose *pose_iter = 0;
	if(pose->m_global_pose_matrices){
		for(pose_iter = pose->m_global_pose_matrices, i = 0;
				i < pose->m_skeleton->num_joints;
				pose_iter++, i++)
			free_joint_pose(pose_iter);
		free(pose->m_global_pose_matrices);
	}
	pose->m_global_pose_matrices = malloc(sizeof(joint_pose) * pose->m_skeleton->num_joints);
	for(pose_iter = pose->m_global_pose_matrices, i = 0;
			i < pose->m_skeleton->num_joints;
			pose_iter++, i++){
		build_global_transformation_poses(*pose, i);
	}
}
unsigned int build_skinning_matrices(mat4 *skinning_matrices, skeleton_pose pose)
{
	unsigned int num_matrices = pose.m_skeleton->num_joints;
	skinning_matrices = malloc(sizeof(mat4) * num_matrices);
	mat4 skinning_matrix = 0;
	unsigned int i = 0;
	for(skinning_matrix = skinning_matrices, i = 0;
			i < num_matrices;
			skinning_matrix++, i++){
		*skinning_matrix = mult_mat4(
				pose.m_skeleton->joints[i].inverse_bind_matrix,
				pose.m_global_pose_matrices[i]);
	}
	return num_matrices;
}
static void build_global_transformation_poses(skeleton_pose *pose, unsigned int i)
//	Recursive function that will fill out the
//		global pose matrices of joint 'i' and it's parents
{
	if(pose->m_global_pose_matrices[i] != 0)
		return;
	unsigned int parent_index = pose->m_local_poses[i].parent_index;
	mat4 local_matrix = build_joint_pose_matrix(pose->m_local_poses[i]);
	if(parent_index == -1){
		pose->m_global_pose_matrices[i] = local_matrix;
		return;
	}
	mat4 *parent_matrix = pose->m_global_poses + parent_index;
	if(parent_matrix == 0){
		build_global_transformation_poses(pose, parent_index)
		parent_matrix = pose->m_global_pose_matrices + parent_index;
	}
	pose->m_global_pose_matrices[i] = mult_mat4(*parent_matrix, local_matrix);
}
