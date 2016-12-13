#include "skeleton.h"
#include "joint.h"
#include <stdlib.h>


skeleton gen_skeleton(void)
{
	skeleton ske;
	ske.joints = 0;
	ske.num_joints = 0;
}
skeleton build_skeleton(unsigned int num_joints)
{
	skeleton ske = gen_skeleton();
	ske.num_joints = num_joints;
	ske.joints = malloc(sizeof(joint) * num_joints);
	return ske;
}
void free_skeleton(skeleton *ske)
{
	if(ske->num_joints){
		free_joint(ske->joints);
		free(ske->joints);
		ske->num_joints = 0;
	}
}

