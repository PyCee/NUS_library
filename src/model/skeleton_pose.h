#ifndef _SKELETON_POSE_H_
#define _SKELETON_POSE_H_

struct skeleton;
struct joint_pose;
struct mat4;

typedef struct skeleton_pose{
	struct skeleton *m_skeleton;
	struct joint_pose *m_local_poses;
	struct mat4 *m_global_pose_matrices;
} skeleton_pose;

skeleton_pose gen_skeleton_pose(void);
void rebuild_skeleton_pose_global_poses(sleleton_pose *);
unsigned int build_skinning_matrices(struct mat4 *, skeleton_pose);

#endif /* _SKELETON_POSE_H_ */
