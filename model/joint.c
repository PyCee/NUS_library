#include "joint.h"
#include "matrix.h"
#include <stdlib.h>
#include <string.h>

joint gen_joint(void)
{
  joint joint_;
  joint_.m_inverse_bind_matrix = build_identity_matrix();
  joint_.m_parent_index = -1;
  joint_.m_name = 0;
  return joint_;
}
joint build_joint(matrix mat, unsigned char parent, const char *name)
{
  joint joint_ = gen_joint();
  joint_.m_inverse_bind_matrix = mat;
  joint_.m_parent_index = parent;
  joint_.m_name = malloc(strlen(name) * 1);
  strcpy(joint_.m_name, name);
  return joint_;
}
void free_joint(joint *joint_)
{
  if(joint_->m_name){
    free(joint_->m_name);
    joint_->m_name = 0;
  }
  joint_->m_parent_index = -1;
}
