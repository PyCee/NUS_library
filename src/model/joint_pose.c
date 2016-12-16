#include "joint_pose.h"
#include "vector.h"
#include "quaternion.h"
#include <stdlib.h>

joint gen_joint_pose(void)
{
	joint_pose pose;
	pose.rotation = malloc(sizeof(quaternion));
	*pose.rotation = build_unit_quaternion(build_vector(0.0, 0.0, 1.0), 0.0);
	pose.translation = malloc(sizeof(vector));
	*pose.translation = build_vector(0.0, 0.0, 0.0);
	pose.scale = 1.0;
	return pose;
}
joint build_joint_pose(quaternion rot, vector tran, float scale)
{
	joint_pose pose = gen_joint_pose();
	*pose.rotation = rot;
	*pose.translation = trans;
	pose.scale = scale;
	return pose;
}
void free_joint_post(joint_pose *pose)
{
	if(pose->rotation){
		free(pose->rotation);
		pose->rotation = 0;
	}
	if(pose->translation){
		free(pose->translation);
		pose->translation = 0;
	}
	pose->scale = 0.0;
}
matrix build_joint_pose_matrix(joint_pose pose)
{
	float sca[16] = {
		pose.scale, 0.0, 0.0, 0.0,
		0.0, pose.scale, 0.0, 0.0,
		0.0, 0.0, pose.scale, 0.0,
		0.0, 0.0, 0.0, 1.0)
	}
	matrix scale = build_matrix(4, 4, sca);
	matrix qua_rot = build_quaternion_matrix44(pose.rotation);
	rotate = multiply_matrices(scale, qua_rot);
	free_quaternion(&qua_rot);
	matrix translate = build_translated_matrix44(pose.translation);
	transform = multiply_matrices(local_pose, translate);
	free_matrix(&scale);
	free_matrix(&rotate);
	free_matrix(&translate);
	return trans;
}
joint_pose interpolate_joint_poses(joint_pose p1, joint_pose p2, float b)
{
	b = (b > 1) ? 1 : b;
	b = (b < 0) ? 0 : b;
	
//	lerp vs slerp
	quaterinon qua = lerp_quaternion(*p1.rotation, *p2.rotation, b);
	//quaterinon qua = slerp_quaternion(*p1.rotation, *p2.rotation, b);
	vector v = add_vector(scale_vector(*p1.translation, 1-b), scale_vector(*p2.translation, b));
	float scale_range = p2.scale - p1.scale;
	return build_joint_pose(qua, v, p1.scale + scale_range * b);
}
