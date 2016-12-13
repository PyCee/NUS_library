#ifndef _SKELETON_H_
#define _SKELETON_H_

struct joint;

typedef struct skeleton{
	struct joint *joints;
	unsigned int num_joints;
} skeleton;

skeleton gen_skeleton(void);
skeleton build_skeleton(unsigned int);
void free_skeleton(skeleton *);


#endif /* _SKELETON_H_ */
