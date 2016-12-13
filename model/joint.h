#ifndef _JOINT_H_
#define _JOINT_H_

#include <matrix.h>
 
typedef struct joint{
  matrix m_inverse_bind_matrix;
  char m_parent_index, *m_name;
} joint;

joint gen_joint(void);
joint build_joint(matrix, unsigned char, const char *);
void free_joint(joint *);

#endif /* _JOINT_H_ */
