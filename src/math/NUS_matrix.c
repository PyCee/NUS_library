#include "NUS_matrix.h"
#include "NUS_vector.h"
#include "NUS_axes.h"
#include <stdio.h>

NUS_matrix nus_matrix_build(double m00, double m01, double m02, double m03,
			    double m10, double m11, double m12, double m13,
			    double m20, double m21, double m22, double m23,
			    double m30, double m31, double m32, double m33)
{
  return (NUS_matrix){{{m00, m01, m02, m03}, {m10, m11, m12, m13},
					       {m20, m21, m22, m23},
						 {m30, m31, m32, m33}}};
}
NUS_matrix nus_matrix_identity(void)
{
  return nus_matrix_build(1.0, 0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0, 0.0,
			  0.0, 0.0, 1.0, 0.0,
			  0.0, 0.0, 0.0, 1.0);
}
NUS_matrix nus_matrix_zero(void)
{
  return nus_matrix_build(0.0, 0.0, 0.0, 0.0,
			  0.0, 0.0, 0.0, 0.0,
			  0.0, 0.0, 0.0, 0.0,
			  0.0, 0.0, 0.0, 0.0);
}
NUS_matrix nus_matrix_transpose(const NUS_matrix NUS_matrix_)
{
  return nus_matrix_build(NUS_matrix_.ele[0][0], NUS_matrix_.ele[1][0],
			  NUS_matrix_.ele[2][0], NUS_matrix_.ele[3][0],
			  NUS_matrix_.ele[0][1], NUS_matrix_.ele[1][1],
			  NUS_matrix_.ele[2][1], NUS_matrix_.ele[3][1],
			  NUS_matrix_.ele[0][2], NUS_matrix_.ele[1][2],
			  NUS_matrix_.ele[2][2], NUS_matrix_.ele[3][2],
			  NUS_matrix_.ele[0][3], NUS_matrix_.ele[1][3],
			  NUS_matrix_.ele[2][3], NUS_matrix_.ele[3][3]);
}
NUS_matrix nus_matrix_scale(const NUS_matrix NUS_matrix_, const double s)
{
  return nus_matrix_build(NUS_matrix_.ele[0][0] * s, NUS_matrix_.ele[0][1] * s,
			  NUS_matrix_.ele[0][2] * s, NUS_matrix_.ele[0][3] * s,
			  NUS_matrix_.ele[1][0] * s, NUS_matrix_.ele[1][1] * s,
			  NUS_matrix_.ele[1][2] * s, NUS_matrix_.ele[1][3] * s,
			  NUS_matrix_.ele[2][0] * s, NUS_matrix_.ele[2][1] * s,
			  NUS_matrix_.ele[2][2] * s, NUS_matrix_.ele[2][3] * s,
			  NUS_matrix_.ele[3][0] * s, NUS_matrix_.ele[3][1] * s,
			  NUS_matrix_.ele[3][2] * s, NUS_matrix_.ele[3][3] * s);
}
NUS_matrix nus_matrix_multiply
(const NUS_matrix NUS_matrix_1, const NUS_matrix NUS_matrix_2)
{
  /*
  unsigned char i, j, k;
  NUS_matrix NUS_matrix = nus_matrix_zero();
  for(i = 0; i < 4; ++i){
    for(j = 0; j < 4; ++j){
      for(k = 0; k < 4; ++k){
	NUS_matrix_.ele[i][j] += NUS_matrix_1.ele[i][k] * NUS_matrix_2.ele[k][j];
      }
    }
  }
  return NUS_matrix_;
  */
  /* The below is the unrolled version of the above */
  return nus_matrix_build(/* First row, first element */
			  NUS_matrix_1.ele[0][0] * NUS_matrix_2.ele[0][0] + 
			  NUS_matrix_1.ele[0][1] * NUS_matrix_2.ele[1][0] + 
			  NUS_matrix_1.ele[0][2] * NUS_matrix_2.ele[2][0] + 
			  NUS_matrix_1.ele[0][3] * NUS_matrix_2.ele[3][0],
			  /* First row, second element */
			  NUS_matrix_1.ele[0][0] * NUS_matrix_2.ele[0][1] + 
			  NUS_matrix_1.ele[0][1] * NUS_matrix_2.ele[1][1] + 
			  NUS_matrix_1.ele[0][2] * NUS_matrix_2.ele[2][1] + 
			  NUS_matrix_1.ele[0][3] * NUS_matrix_2.ele[3][1],
			  /* First row, third element */
			  NUS_matrix_1.ele[0][0] * NUS_matrix_2.ele[0][2] + 
			  NUS_matrix_1.ele[0][1] * NUS_matrix_2.ele[1][2] + 
			  NUS_matrix_1.ele[0][2] * NUS_matrix_2.ele[2][2] + 
			  NUS_matrix_1.ele[0][3] * NUS_matrix_2.ele[3][2],
			  /* First row, fourth element */
			  NUS_matrix_1.ele[0][0] * NUS_matrix_2.ele[0][3] + 
			  NUS_matrix_1.ele[0][1] * NUS_matrix_2.ele[1][3] + 
			  NUS_matrix_1.ele[0][2] * NUS_matrix_2.ele[2][3] + 
			  NUS_matrix_1.ele[0][3] * NUS_matrix_2.ele[3][3],
			  /* Second row, first element */
			  NUS_matrix_1.ele[1][0] * NUS_matrix_2.ele[0][0] + 
			  NUS_matrix_1.ele[1][1] * NUS_matrix_2.ele[1][0] + 
			  NUS_matrix_1.ele[1][2] * NUS_matrix_2.ele[2][0] + 
			  NUS_matrix_1.ele[1][3] * NUS_matrix_2.ele[3][0],
			  /* Second row, second element */
			  NUS_matrix_1.ele[1][0] * NUS_matrix_2.ele[0][1] + 
			  NUS_matrix_1.ele[1][1] * NUS_matrix_2.ele[1][1] + 
			  NUS_matrix_1.ele[1][2] * NUS_matrix_2.ele[2][1] + 
			  NUS_matrix_1.ele[1][3] * NUS_matrix_2.ele[3][1],
			  /* Second row, third element */
			  NUS_matrix_1.ele[1][0] * NUS_matrix_2.ele[0][2] + 
			  NUS_matrix_1.ele[1][1] * NUS_matrix_2.ele[1][2] + 
			  NUS_matrix_1.ele[1][2] * NUS_matrix_2.ele[2][2] + 
			  NUS_matrix_1.ele[1][3] * NUS_matrix_2.ele[3][2],
			  /* Second row, fourth element */
			  NUS_matrix_1.ele[1][0] * NUS_matrix_2.ele[0][3] + 
			  NUS_matrix_1.ele[1][1] * NUS_matrix_2.ele[1][3] + 
			  NUS_matrix_1.ele[1][2] * NUS_matrix_2.ele[2][3] + 
			  NUS_matrix_1.ele[1][3] * NUS_matrix_2.ele[3][3],
			  /* Third row, first element */
			  NUS_matrix_1.ele[2][0] * NUS_matrix_2.ele[0][0] + 
			  NUS_matrix_1.ele[2][1] * NUS_matrix_2.ele[1][0] + 
			  NUS_matrix_1.ele[2][2] * NUS_matrix_2.ele[2][0] + 
			  NUS_matrix_1.ele[2][3] * NUS_matrix_2.ele[3][0],
			  /* Third row, second element */
			  NUS_matrix_1.ele[2][0] * NUS_matrix_2.ele[0][1] + 
			  NUS_matrix_1.ele[2][1] * NUS_matrix_2.ele[1][1] + 
			  NUS_matrix_1.ele[2][2] * NUS_matrix_2.ele[2][1] + 
			  NUS_matrix_1.ele[2][3] * NUS_matrix_2.ele[3][1],
			  /* Third row, third element */
			  NUS_matrix_1.ele[2][0] * NUS_matrix_2.ele[0][2] + 
			  NUS_matrix_1.ele[2][1] * NUS_matrix_2.ele[1][2] + 
			  NUS_matrix_1.ele[2][2] * NUS_matrix_2.ele[2][2] + 
			  NUS_matrix_1.ele[2][3] * NUS_matrix_2.ele[3][2],
			  /* Third row, fourth element */
			  NUS_matrix_1.ele[2][0] * NUS_matrix_2.ele[0][3] + 
			  NUS_matrix_1.ele[2][1] * NUS_matrix_2.ele[1][3] + 
			  NUS_matrix_1.ele[2][2] * NUS_matrix_2.ele[2][3] + 
			  NUS_matrix_1.ele[2][3] * NUS_matrix_2.ele[3][3],
			  /* Fourth row, first element */
			  NUS_matrix_1.ele[3][0] * NUS_matrix_2.ele[0][0] + 
			  NUS_matrix_1.ele[3][1] * NUS_matrix_2.ele[1][0] + 
			  NUS_matrix_1.ele[3][2] * NUS_matrix_2.ele[2][0] + 
			  NUS_matrix_1.ele[3][3] * NUS_matrix_2.ele[3][0],
			  /* Fourth row, second element */
			  NUS_matrix_1.ele[3][0] * NUS_matrix_2.ele[0][1] + 
			  NUS_matrix_1.ele[3][1] * NUS_matrix_2.ele[1][1] + 
			  NUS_matrix_1.ele[3][2] * NUS_matrix_2.ele[2][1] + 
			  NUS_matrix_1.ele[3][3] * NUS_matrix_2.ele[3][1],
			  /* Fourth row, third element */
			  NUS_matrix_1.ele[3][0] * NUS_matrix_2.ele[0][2] + 
			  NUS_matrix_1.ele[3][1] * NUS_matrix_2.ele[1][2] + 
			  NUS_matrix_1.ele[3][2] * NUS_matrix_2.ele[2][2] + 
			  NUS_matrix_1.ele[3][3] * NUS_matrix_2.ele[3][2],
			  /* Fourth row, fourth element */
			  NUS_matrix_1.ele[3][0] * NUS_matrix_2.ele[0][3] + 
			  NUS_matrix_1.ele[3][1] * NUS_matrix_2.ele[1][3] + 
			  NUS_matrix_1.ele[3][2] * NUS_matrix_2.ele[2][3] + 
			  NUS_matrix_1.ele[3][3] * NUS_matrix_2.ele[3][3]);
}
NUS_matrix nus_matrix_translation(const struct NUS_vector NUS_vector_)
{
  return nus_matrix_build(1.0, 0.0, 0.0, NUS_vector_.x,
			  0.0, 1.0, 0.0, NUS_vector_.y,
			  0.0, 0.0, 1.0, NUS_vector_.z,
			  0.0, 0.0, 0.0, 1.0);
}
NUS_matrix nus_matrix_rotation(struct NUS_axes NUS_axes_)
{
  return nus_matrix_build(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
  /* Below is a quaternion --> rotation matrix algorithm */
  /*double w = NUS_quaternion_.w,
    x = NUS_quaternion_.x,
    y = NUS_quaternion_.y,
    z = NUS_quaternion_.z;
  return nus_matrix_build(1.0 - 2.0 * (y*y-z*z), 2.0 * (x*y-z*w), 2.0 * (x*z+y*w), 0.0,
			  2.0 * (x*y+z*w), 1.0 - 2.0 * (x*x-z*z), 2.0 * (y*z-x*w), 0.0,
			  2.0 * (x*z-y*w), 2.0 * (y*z+x*w), 1.0 - 2.0 * (x*x-y*y), 0.0,
			  0.0, 0.0, 0.0, 1.0);
  */
}

/* TODO: complete the below functions */
NUS_matrix nus_matrix_transformation
(struct NUS_vector, struct NUS_axes);

NUS_matrix nus_matrix_inverted(NUS_matrix);

void nus_matrix_print(NUS_matrix NUS_matrix_)
{
  printf("NUS_matrix:\n\
{%f, %f, %f, %f,\n %f, %f, %f, %f,\n %f, %f, %f, %f,\n %f, %f, %f, %f}\n",
	 NUS_matrix_.ele[0][0], NUS_matrix_.ele[0][1],
	 NUS_matrix_.ele[0][2], NUS_matrix_.ele[0][3],
	 NUS_matrix_.ele[1][0], NUS_matrix_.ele[1][1],
	 NUS_matrix_.ele[1][2], NUS_matrix_.ele[1][3],
	 NUS_matrix_.ele[2][0], NUS_matrix_.ele[2][1],
	 NUS_matrix_.ele[2][2], NUS_matrix_.ele[2][3],
	 NUS_matrix_.ele[3][0], NUS_matrix_.ele[3][1],
	 NUS_matrix_.ele[3][2], NUS_matrix_.ele[3][3]);
}
