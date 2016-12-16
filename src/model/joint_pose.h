#ifndef _JOINT_POSE_H_
#define _JOINT_POSE_H_

struct quaternion;
struct vector;

typedef struct joint_pose{
	struct quaternion *rotation;
	struct vector *translation;
	float scale;
} joint_pose;

joint_pose gen_joint_pose(void);
joint_pose build_joint_pose(struct quaternion, struct vector, float);
void free_joint_post(joint_pose *);
mat4 build_joint_pose_matrix(joint_pose);
joint_pose interpolate_joint_poses(joint_pose, joint_pose, float);

#endif /* _JOINT_POSE_H_ */
