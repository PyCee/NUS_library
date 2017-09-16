#include "NUS_matrix.h"
#include "NUS_vector.h"
#include "NUS_axes.h"
#include "../NUS_log.h"
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
NUS_matrix nus_matrix_build_translation(const struct NUS_vector vector)
{
  return nus_matrix_build(1.0, 0.0, 0.0, vector.x,
			  0.0, 1.0, 0.0, vector.y,
			  0.0, 0.0, 1.0, vector.z,
			  0.0, 0.0, 0.0, 1.0);
}
NUS_matrix nus_matrix_build_scale(NUS_vector scale)
{
  return nus_matrix_build(scale.x, 0.0, 0.0, 0.0,
			  0.0, scale.y, 0.0, 0.0,
			  0.0, 0.0, scale.z, 0.0,
			  0.0, 0.0, 0.0, 1.0);
}
NUS_matrix nus_matrix_build_rotation(NUS_axes axes)
{
  return nus_matrix_build(axes.right.x, axes.right.y, axes.right.z, 0.0,
			  axes.upward.x, axes.upward.y, axes.upward.z, 0.0,
			  axes.forward.x, axes.forward.y, axes.forward.z, 0.0,
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
			  0.0, 0.0, 0.0, 1.0); */
}

NUS_matrix nus_matrix_build_transformation
(NUS_vector vector, NUS_vector scale, NUS_axes axes)
{
  return nus_matrix_multiply(nus_matrix_build_translation(vector),
			     nus_matrix_multiply(nus_matrix_build_rotation(axes),
						 nus_matrix_build_scale(scale)));
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
NUS_matrix nus_matrix_inverse(NUS_matrix matrix)
{
  NUS_matrix inverse;
  double inv[16], det;
  int i, j;
  
  inv[0] = matrix.ele[1][1]  * matrix.ele[2][2] * matrix.ele[3][3] - 
    matrix.ele[1][1]  * matrix.ele[2][3] * matrix.ele[3][2] - 
    matrix.ele[2][1]  * matrix.ele[1][2]  * matrix.ele[3][3] + 
    matrix.ele[2][1]  * matrix.ele[1][3]  * matrix.ele[3][2] +
    matrix.ele[3][1] * matrix.ele[1][2]  * matrix.ele[2][3] - 
    matrix.ele[3][1] * matrix.ele[1][3]  * matrix.ele[2][2];
  
  inv[4] = -matrix.ele[1][0]  * matrix.ele[2][2] * matrix.ele[3][3] + 
    matrix.ele[1][0]  * matrix.ele[2][3] * matrix.ele[3][2] + 
    matrix.ele[2][0]  * matrix.ele[1][2]  * matrix.ele[3][3] - 
    matrix.ele[2][0]  * matrix.ele[1][3]  * matrix.ele[3][2] - 
    matrix.ele[3][0] * matrix.ele[1][2]  * matrix.ele[2][3] + 
    matrix.ele[3][0] * matrix.ele[1][3]  * matrix.ele[2][2];
  
  inv[8] = matrix.ele[1][0]  * matrix.ele[2][1] * matrix.ele[3][3] - 
    matrix.ele[1][0]  * matrix.ele[2][3] * matrix.ele[3][1] - 
    matrix.ele[2][0]  * matrix.ele[1][1] * matrix.ele[3][3] + 
    matrix.ele[2][0]  * matrix.ele[1][3] * matrix.ele[3][1] + 
    matrix.ele[3][0] * matrix.ele[1][1] * matrix.ele[2][3] - 
    matrix.ele[3][0] * matrix.ele[1][3] * matrix.ele[2][1];
  
  inv[12] = -matrix.ele[1][0]  * matrix.ele[2][1] * matrix.ele[3][2] + 
    matrix.ele[1][0]  * matrix.ele[2][2] * matrix.ele[3][1] +
    matrix.ele[2][0]  * matrix.ele[1][1] * matrix.ele[3][2] - 
    matrix.ele[2][0]  * matrix.ele[1][2] * matrix.ele[3][1] - 
    matrix.ele[3][0] * matrix.ele[1][1] * matrix.ele[2][2] + 
    matrix.ele[3][0] * matrix.ele[1][2] * matrix.ele[2][1];
  
  inv[1] = -matrix.ele[0][1]  * matrix.ele[2][2] * matrix.ele[3][3] + 
    matrix.ele[0][1]  * matrix.ele[2][3] * matrix.ele[3][2] + 
    matrix.ele[2][1]  * matrix.ele[0][2] * matrix.ele[3][3] - 
    matrix.ele[2][1]  * matrix.ele[0][3] * matrix.ele[3][2] - 
    matrix.ele[3][1] * matrix.ele[0][2] * matrix.ele[2][3] + 
    matrix.ele[3][1] * matrix.ele[0][3] * matrix.ele[2][2];
  
  inv[5] = matrix.ele[0][0]  * matrix.ele[2][2] * matrix.ele[3][3] - 
    matrix.ele[0][0]  * matrix.ele[2][3] * matrix.ele[3][2] - 
    matrix.ele[2][0]  * matrix.ele[0][2] * matrix.ele[3][3] + 
    matrix.ele[2][0]  * matrix.ele[0][3] * matrix.ele[3][2] + 
    matrix.ele[3][0] * matrix.ele[0][2] * matrix.ele[2][3] - 
    matrix.ele[3][0] * matrix.ele[0][3] * matrix.ele[2][2];
  
  inv[9] = -matrix.ele[0][0]  * matrix.ele[2][1] * matrix.ele[3][3] + 
    matrix.ele[0][0]  * matrix.ele[2][3] * matrix.ele[3][1] + 
    matrix.ele[2][0]  * matrix.ele[0][1] * matrix.ele[3][3] - 
    matrix.ele[2][0]  * matrix.ele[0][3] * matrix.ele[3][1] - 
    matrix.ele[3][0] * matrix.ele[0][1] * matrix.ele[2][3] + 
    matrix.ele[3][0] * matrix.ele[0][3] * matrix.ele[2][1];
  
  inv[13] = matrix.ele[0][0]  * matrix.ele[2][1] * matrix.ele[3][2] - 
    matrix.ele[0][0]  * matrix.ele[2][2] * matrix.ele[3][1] - 
    matrix.ele[2][0]  * matrix.ele[0][1] * matrix.ele[3][2] + 
    matrix.ele[2][0]  * matrix.ele[0][2] * matrix.ele[3][1] + 
    matrix.ele[3][0] * matrix.ele[0][1] * matrix.ele[2][2] - 
    matrix.ele[3][0] * matrix.ele[0][2] * matrix.ele[2][1];
  
  inv[2] = matrix.ele[0][1]  * matrix.ele[1][2] * matrix.ele[3][3] - 
    matrix.ele[0][1]  * matrix.ele[1][3] * matrix.ele[3][2] - 
    matrix.ele[1][1]  * matrix.ele[0][2] * matrix.ele[3][3] + 
    matrix.ele[1][1]  * matrix.ele[0][3] * matrix.ele[3][2] + 
    matrix.ele[3][1] * matrix.ele[0][2] * matrix.ele[1][3] - 
    matrix.ele[3][1] * matrix.ele[0][3] * matrix.ele[1][2];
  
  inv[6] = -matrix.ele[0][0]  * matrix.ele[1][2] * matrix.ele[3][3] + 
    matrix.ele[0][0]  * matrix.ele[1][3] * matrix.ele[3][2] + 
    matrix.ele[1][0]  * matrix.ele[0][2] * matrix.ele[3][3] - 
    matrix.ele[1][0]  * matrix.ele[0][3] * matrix.ele[3][2] - 
    matrix.ele[3][0] * matrix.ele[0][2] * matrix.ele[1][3] + 
    matrix.ele[3][0] * matrix.ele[0][3] * matrix.ele[1][2];
  
  inv[10] = matrix.ele[0][0]  * matrix.ele[1][1] * matrix.ele[3][3] - 
    matrix.ele[0][0]  * matrix.ele[1][3] * matrix.ele[3][1] - 
    matrix.ele[1][0]  * matrix.ele[0][1] * matrix.ele[3][3] + 
    matrix.ele[1][0]  * matrix.ele[0][3] * matrix.ele[3][1] + 
    matrix.ele[3][0] * matrix.ele[0][1] * matrix.ele[1][3] - 
    matrix.ele[3][0] * matrix.ele[0][3] * matrix.ele[1][1];
  
  inv[14] = -matrix.ele[0][0]  * matrix.ele[1][1] * matrix.ele[3][2] + 
    matrix.ele[0][0]  * matrix.ele[1][2] * matrix.ele[3][1] + 
    matrix.ele[1][0]  * matrix.ele[0][1] * matrix.ele[3][2] - 
    matrix.ele[1][0]  * matrix.ele[0][2] * matrix.ele[3][1] - 
    matrix.ele[3][0] * matrix.ele[0][1] * matrix.ele[1][2] + 
    matrix.ele[3][0] * matrix.ele[0][2] * matrix.ele[1][1];
  
  inv[3] = -matrix.ele[0][1] * matrix.ele[1][2] * matrix.ele[2][3] + 
    matrix.ele[0][1] * matrix.ele[1][3] * matrix.ele[2][2] + 
    matrix.ele[1][1] * matrix.ele[0][2] * matrix.ele[2][3] - 
    matrix.ele[1][1] * matrix.ele[0][3] * matrix.ele[2][2] - 
    matrix.ele[2][1] * matrix.ele[0][2] * matrix.ele[1][3] + 
    matrix.ele[2][1] * matrix.ele[0][3] * matrix.ele[1][2];
  
  inv[7] = matrix.ele[0][0] * matrix.ele[1][2] * matrix.ele[2][3] - 
    matrix.ele[0][0] * matrix.ele[1][3] * matrix.ele[2][2] - 
    matrix.ele[1][0] * matrix.ele[0][2] * matrix.ele[2][3] + 
    matrix.ele[1][0] * matrix.ele[0][3] * matrix.ele[2][2] + 
    matrix.ele[2][0] * matrix.ele[0][2] * matrix.ele[1][3] - 
    matrix.ele[2][0] * matrix.ele[0][3] * matrix.ele[1][2];
  
  inv[11] = -matrix.ele[0][0] * matrix.ele[1][1] * matrix.ele[2][3] + 
    matrix.ele[0][0] * matrix.ele[1][3] * matrix.ele[2][1] + 
    matrix.ele[1][0] * matrix.ele[0][1] * matrix.ele[2][3] - 
    matrix.ele[1][0] * matrix.ele[0][3] * matrix.ele[2][1] - 
    matrix.ele[2][0] * matrix.ele[0][1] * matrix.ele[1][3] + 
    matrix.ele[2][0] * matrix.ele[0][3] * matrix.ele[1][1];
  
  inv[15] = matrix.ele[0][0] * matrix.ele[1][1] * matrix.ele[2][2] - 
    matrix.ele[0][0] * matrix.ele[1][2] * matrix.ele[2][1] - 
    matrix.ele[1][0] * matrix.ele[0][1] * matrix.ele[2][2] + 
    matrix.ele[1][0] * matrix.ele[0][2] * matrix.ele[2][1] + 
    matrix.ele[2][0] * matrix.ele[0][1] * matrix.ele[1][2] - 
    matrix.ele[2][0] * matrix.ele[0][2] * matrix.ele[1][1];

  det = matrix.ele[0][0] * inv[0] + matrix.ele[0][1] * inv[4] +
    matrix.ele[0][2] * inv[8] + matrix.ele[0][3] * inv[12];
  
  if (det * det < 0.00001){
    NUS_LOG_ERROR("inverting matrix that is not invertable\n");
    return nus_matrix_identity();
  }
  
  det = 1.0 / det;
  
  for(i = 0; i < 4; ++i){
    for(j = 0; j < 4; ++j){
      inverse.ele[i][j] = inv[4 * i + j] * det;
    }
  }
  
  return inverse;
}
NUS_bool nus_matrix_cmp(NUS_matrix matrix_0, NUS_matrix matrix_1, float d)
{
  float sum_of_differences = (matrix_0.ele[0][0] - matrix_1.ele[0][0]) +
    (matrix_0.ele[0][1] - matrix_1.ele[0][1]) +
    (matrix_0.ele[0][2] - matrix_1.ele[0][2]) +
    (matrix_0.ele[0][3] - matrix_1.ele[0][3]) +
    (matrix_0.ele[1][0] - matrix_1.ele[1][0]) +
    (matrix_0.ele[1][1] - matrix_1.ele[1][1]) +
    (matrix_0.ele[1][2] - matrix_1.ele[1][2]) +
    (matrix_0.ele[1][3] - matrix_1.ele[1][3]) +
    (matrix_0.ele[2][0] - matrix_1.ele[2][0]) +
    (matrix_0.ele[2][1] - matrix_1.ele[2][1]) +
    (matrix_0.ele[2][2] - matrix_1.ele[2][2]) +
    (matrix_0.ele[2][3] - matrix_1.ele[2][3]) +
    (matrix_0.ele[3][0] - matrix_1.ele[3][0]) +
    (matrix_0.ele[3][1] - matrix_1.ele[3][1]) +
    (matrix_0.ele[3][2] - matrix_1.ele[3][2]) +
    (matrix_0.ele[3][3] - matrix_1.ele[3][3]);
  if(sum_of_differences < (16 * d) && sum_of_differences > -1.0 * (16 * d)){
    return NUS_TRUE;
  }
  return NUS_FALSE;
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
