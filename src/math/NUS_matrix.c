#include "NUS_matrix.h"
#include "NUS_vector.h"
#include "NUS_axes.h"
#include <stdio.h>

NUS_matrix nus_matrix_build(float m00, float m01, float m02, float m03,
			    float m10, float m11, float m12, float m13,
			    float m20, float m21, float m22, float m23,
			    float m30, float m31, float m32, float m33)
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
NUS_matrix nus_matrix_transpose(const NUS_matrix matrix)
{
  return nus_matrix_build(matrix.ele[0][0], matrix.ele[1][0],
			  matrix.ele[2][0], matrix.ele[3][0],
			  matrix.ele[0][1], matrix.ele[1][1],
			  matrix.ele[2][1], matrix.ele[3][1],
			  matrix.ele[0][2], matrix.ele[1][2],
			  matrix.ele[2][2], matrix.ele[3][2],
			  matrix.ele[0][3], matrix.ele[1][3],
			  matrix.ele[2][3], matrix.ele[3][3]);
}
NUS_matrix nus_matrix_scale(const NUS_matrix matrix, const float s)
{
  return nus_matrix_build(matrix.ele[0][0] * s, matrix.ele[0][1] * s,
			  matrix.ele[0][2] * s, matrix.ele[0][3] * s,
			  matrix.ele[1][0] * s, matrix.ele[1][1] * s,
			  matrix.ele[1][2] * s, matrix.ele[1][3] * s,
			  matrix.ele[2][0] * s, matrix.ele[2][1] * s,
			  matrix.ele[2][2] * s, matrix.ele[2][3] * s,
			  matrix.ele[3][0] * s, matrix.ele[3][1] * s,
			  matrix.ele[3][2] * s, matrix.ele[3][3] * s);
}
NUS_matrix nus_matrix_multiply
(const NUS_matrix matrix_0, const NUS_matrix matrix_1)
{
  /* matrix multiplication-loop form */
  /*
  unsigned char i, j, k;
  NUS_matrix NUS_matrix = nus_matrix_zero();
  for(i = 0; i < 4; ++i){
    for(j = 0; j < 4; ++j){
      for(k = 0; k < 4; ++k){
	matrix.ele[i][j] += matrix_0.ele[i][k] * matrix_1.ele[k][j];
      }
    }
  }
  return matrix;
  */
  /* matrix multiplication-unrolled*/
  return nus_matrix_build(/* First row, first element */
			  matrix_0.ele[0][0] * matrix_1.ele[0][0] + 
			  matrix_0.ele[0][1] * matrix_1.ele[1][0] + 
			  matrix_0.ele[0][2] * matrix_1.ele[2][0] + 
			  matrix_0.ele[0][3] * matrix_1.ele[3][0],
			  /* First row, second element */
			  matrix_0.ele[0][0] * matrix_1.ele[0][1] + 
			  matrix_0.ele[0][1] * matrix_1.ele[1][1] + 
			  matrix_0.ele[0][2] * matrix_1.ele[2][1] + 
			  matrix_0.ele[0][3] * matrix_1.ele[3][1],
			  /* First row, third element */
			  matrix_0.ele[0][0] * matrix_1.ele[0][2] + 
			  matrix_0.ele[0][1] * matrix_1.ele[1][2] + 
			  matrix_0.ele[0][2] * matrix_1.ele[2][2] + 
			  matrix_0.ele[0][3] * matrix_1.ele[3][2],
			  /* First row, fourth element */
			  matrix_0.ele[0][0] * matrix_1.ele[0][3] + 
			  matrix_0.ele[0][1] * matrix_1.ele[1][3] + 
			  matrix_0.ele[0][2] * matrix_1.ele[2][3] + 
			  matrix_0.ele[0][3] * matrix_1.ele[3][3],
			  /* Second row, first element */
			  matrix_0.ele[1][0] * matrix_1.ele[0][0] + 
			  matrix_0.ele[1][1] * matrix_1.ele[1][0] + 
			  matrix_0.ele[1][2] * matrix_1.ele[2][0] + 
			  matrix_0.ele[1][3] * matrix_1.ele[3][0],
			  /* Second row, second element */
			  matrix_0.ele[1][0] * matrix_1.ele[0][1] + 
			  matrix_0.ele[1][1] * matrix_1.ele[1][1] + 
			  matrix_0.ele[1][2] * matrix_1.ele[2][1] + 
			  matrix_0.ele[1][3] * matrix_1.ele[3][1],
			  /* Second row, third element */
			  matrix_0.ele[1][0] * matrix_1.ele[0][2] + 
			  matrix_0.ele[1][1] * matrix_1.ele[1][2] + 
			  matrix_0.ele[1][2] * matrix_1.ele[2][2] + 
			  matrix_0.ele[1][3] * matrix_1.ele[3][2],
			  /* Second row, fourth element */
			  matrix_0.ele[1][0] * matrix_1.ele[0][3] + 
			  matrix_0.ele[1][1] * matrix_1.ele[1][3] + 
			  matrix_0.ele[1][2] * matrix_1.ele[2][3] + 
			  matrix_0.ele[1][3] * matrix_1.ele[3][3],
			  /* Third row, first element */
			  matrix_0.ele[2][0] * matrix_1.ele[0][0] + 
			  matrix_0.ele[2][1] * matrix_1.ele[1][0] + 
			  matrix_0.ele[2][2] * matrix_1.ele[2][0] + 
			  matrix_0.ele[2][3] * matrix_1.ele[3][0],
			  /* Third row, second element */
			  matrix_0.ele[2][0] * matrix_1.ele[0][1] + 
			  matrix_0.ele[2][1] * matrix_1.ele[1][1] + 
			  matrix_0.ele[2][2] * matrix_1.ele[2][1] + 
			  matrix_0.ele[2][3] * matrix_1.ele[3][1],
			  /* Third row, third element */
			  matrix_0.ele[2][0] * matrix_1.ele[0][2] + 
			  matrix_0.ele[2][1] * matrix_1.ele[1][2] + 
			  matrix_0.ele[2][2] * matrix_1.ele[2][2] + 
			  matrix_0.ele[2][3] * matrix_1.ele[3][2],
			  /* Third row, fourth element */
			  matrix_0.ele[2][0] * matrix_1.ele[0][3] + 
			  matrix_0.ele[2][1] * matrix_1.ele[1][3] + 
			  matrix_0.ele[2][2] * matrix_1.ele[2][3] + 
			  matrix_0.ele[2][3] * matrix_1.ele[3][3],
			  /* Fourth row, first element */
			  matrix_0.ele[3][0] * matrix_1.ele[0][0] + 
			  matrix_0.ele[3][1] * matrix_1.ele[1][0] + 
			  matrix_0.ele[3][2] * matrix_1.ele[2][0] + 
			  matrix_0.ele[3][3] * matrix_1.ele[3][0],
			  /* Fourth row, second element */
			  matrix_0.ele[3][0] * matrix_1.ele[0][1] + 
			  matrix_0.ele[3][1] * matrix_1.ele[1][1] + 
			  matrix_0.ele[3][2] * matrix_1.ele[2][1] + 
			  matrix_0.ele[3][3] * matrix_1.ele[3][1],
			  /* Fourth row, third element */
			  matrix_0.ele[3][0] * matrix_1.ele[0][2] + 
			  matrix_0.ele[3][1] * matrix_1.ele[1][2] + 
			  matrix_0.ele[3][2] * matrix_1.ele[2][2] + 
			  matrix_0.ele[3][3] * matrix_1.ele[3][2],
			  /* Fourth row, fourth element */
			  matrix_0.ele[3][0] * matrix_1.ele[0][3] + 
			  matrix_0.ele[3][1] * matrix_1.ele[1][3] + 
			  matrix_0.ele[3][2] * matrix_1.ele[2][3] + 
			  matrix_0.ele[3][3] * matrix_1.ele[3][3]);
}
NUS_matrix nus_matrix_translation(const struct NUS_vector NUS_vector_)
{
  return nus_matrix_build(1.0, 0.0, 0.0, NUS_vector_.x,
			  0.0, 1.0, 0.0, NUS_vector_.y,
			  0.0, 0.0, 1.0, NUS_vector_.z,
			  0.0, 0.0, 0.0, 1.0);
}
NUS_matrix nus_matrix_rotation(NUS_axes NUS_axes_)
{
  return nus_matrix_build(NUS_axes_.left.x, NUS_axes_.left.y,
			  NUS_axes_.left.z, 0.0,
			  NUS_axes_.upward.x, NUS_axes_.upward.y,
			  NUS_axes_.upward.z, 0.0,
			  NUS_axes_.forward.x, NUS_axes_.forward.y,
			  NUS_axes_.forward.z, 0.0,
			  0.0, 0.0, 0.0, 1.0);
  /* Below is a quaternion --> rotation matrix algorithm.
   a remnant of a past time */
  /*float w = NUS_quaternion_.w,
    x = NUS_quaternion_.x,
    y = NUS_quaternion_.y,
    z = NUS_quaternion_.z;
  return nus_matrix_build(1.0 - 2.0 * (y*y-z*z), 2.0 * (x*y-z*w), 2.0 * (x*z+y*w), 0.0,
			  2.0 * (x*y+z*w), 1.0 - 2.0 * (x*x-z*z), 2.0 * (y*z-x*w), 0.0,
			  2.0 * (x*z-y*w), 2.0 * (y*z+x*w), 1.0 - 2.0 * (x*x-y*y), 0.0,
			  0.0, 0.0, 0.0, 1.0);
  */
}

NUS_matrix nus_matrix_transformation
(NUS_vector vector, NUS_axes axes)
{
  NUS_matrix scale = nus_matrix_scale(nus_matrix_identity(), 1.0),
    rotation = nus_matrix_rotation(axes),
    translation = nus_matrix_translation(vector);
  return nus_matrix_multiply(nus_matrix_multiply(translation, rotation), scale);
}
NUS_vector nus_matrix_transform(NUS_matrix matrix, NUS_vector vector)
{
  return nus_vector_build(/* x component */
			  matrix.ele[0][0] * vector.x +
			  matrix.ele[0][1] * vector.y +
			  matrix.ele[0][2] * vector.z +
			  matrix.ele[0][3] * 1.0,
			  /* y component */
			  matrix.ele[1][0] * vector.x +
			  matrix.ele[1][1] * vector.y +
			  matrix.ele[1][2] * vector.z +
			  matrix.ele[1][3] * 1.0,
			  /* z component */
			  matrix.ele[2][0] * vector.x +
			  matrix.ele[2][1] * vector.y +
			  matrix.ele[2][2] * vector.z +
			  matrix.ele[2][3] * 1.0);
}
NUS_matrix nus_matrix_inverted(NUS_matrix matrix)
{
  return nus_matrix_identity();
}
void nus_matrix_print(NUS_matrix matrix)
{
  printf("{%f, %f, %f, %f,\n %f, %f, %f, %f,\n %f, %f, %f, %f,\n %f, %f, %f, %f}\n",
	 matrix.ele[0][0], matrix.ele[0][1], matrix.ele[0][2],
	 matrix.ele[0][3], matrix.ele[1][0], matrix.ele[1][1],
	 matrix.ele[1][2], matrix.ele[1][3], matrix.ele[2][0],
	 matrix.ele[2][1], matrix.ele[2][2], matrix.ele[2][3],
	 matrix.ele[3][0], matrix.ele[3][1], matrix.ele[3][2],
	 matrix.ele[3][3]);
}
