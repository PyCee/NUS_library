#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "quaternion.h"
#include "vector.h"
#include "cartesian_system.h"

static float determinant_helper(const matrix, char);
matrix gen_matrix(void)
{
  static matrix _matrix = DEFAULT_MATRIX;
  return _matrix;
}
matrix build_matrix(float m00, float m01, float m02, float m03,
		    float m10, float m11, float m12, float m13,
		    float m20, float m21, float m22, float m23,
		    float m30, float m31, float m32, float m33)
{
  matrix _matrix = {{{m00, m01, m02, m03},
		     {m10, m11, m12, m13},
		     {m20, m21, m22, m23},
		     {m30, m31, m32, m33}}};
  return _matrix;
}
matrix build_identity_matrix(void)
{
  static matrix _matrix = {{{1.0, 0.0, 0.0, 0.0},
			    {0.0, 1.0, 0.0, 0.0},
			    {0.0, 0.0, 1.0, 0.0},
			    {0.0, 0.0, 0.0, 1.0}}};
  return _matrix;
}
matrix build_zero_matrix(void)
{
  static matrix _matrix = {{{0.0, 0.0, 0.0, 0.0},
			    {0.0, 0.0, 0.0, 0.0},
			    {0.0, 0.0, 0.0, 0.0},
			    {0.0, 0.0, 0.0, 0.0}}};
  return _matrix;
}
matrix build_inverted_matrix(const matrix _matrix)
{
  float det;
  if((det = matrix_determinant(_matrix)))
    return build_identity_matrix();
  matrix cofactor = build_cofactor_matrix(_matrix),
    adjoint = build_transposed_matrix(cofactor),
    inverse = scale_matrix(adjoint, 1.0/det);
  return inverse;
}
matrix scale_matrix(matrix _matrix, float scale)
{
  unsigned char i, j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++)
      _matrix.ele[i][j] *= scale;
  }
  return _matrix;
}
matrix build_scale_matrix(float scale)
{
  return build_matrix(scale, 0.0, 0.0, 0.0,
		      0.0, scale, 0.0, 0.0,
		      0.0, 0.0, scale, 0.0,
		      0.0, 0.0, 0.0, 1.0);
}
matrix build_transposed_matrix(const matrix _matrix)
{
  unsigned char i, j;
  matrix transposed;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++)
      transposed.ele[i][j] = _matrix.ele[j][i];
  }
  return transposed;
}
matrix multiply_matrices(const matrix m1, const matrix m2)
{
  matrix _matrix = build_zero_matrix();
  unsigned char i, j, k;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){
      for(k = 0; k < 4; k++){
	_matrix.ele[i][j] += m1.ele[i][k] * m2.ele[k][j];
      }
    }
  }
  return _matrix;
}
void print_matrix(const matrix _matrix)
{
  unsigned char i, j;
  printf("[");
  for(i = 0; i < 4; i++){
    printf(" %f", _matrix.ele[i][0]);
    for(j = 1; j < 4; j++){
      printf(", %f", _matrix.ele[i][j]);
    }
    if(i == 3)
      printf("]");
    printf("\n ");
  }
}
matrix build_quaternion_matrix(quaternion qua)
{
  float w = qua.w,
    x = qua.x,
    y = qua.y,
    z = qua.z;
  return build_matrix(1.0 - 2.0 * (y*y-z*z), 2.0 * (x*y-z*w), 2.0 * (x*z+y*w), 0.0,
		      2.0 * (x*y+z*w), 1.0 - 2.0 * (x*x-z*z), 2.0 * (y*z-x*w), 0.0,
		      2.0 * (x*z-y*w), 2.0 * (y*z+x*w), 1.0 - 2.0 * (x*x-y*y), 0.0,
		      0.0, 0.0, 0.0, 1.0);
}
matrix build_cartesian_system_matrix(cartesian_system cartesian_system_)
{
    return build_matrix(cartesian_system_.left.x, cartesian_system_.left.y,
			cartesian_system_.left.z, 0.0,
			cartesian_system_.upward.x, cartesian_system_.upward.y,
			cartesian_system_.upward.z, 0.0,
			cartesian_system_.forward.x, cartesian_system_.forward.y,
			cartesian_system_.forward.z, 0.0,
			0.0, 0.0, 0.0, 1.0);
}
matrix build_translation_matrix(vector v)
{
  matrix _matrix = build_identity_matrix();
  _matrix.ele[0][3] = v.x;
  _matrix.ele[1][3] = v.y;
  _matrix.ele[2][3] = v.z;
  return _matrix;
}
matrix build_transformation_matrix
(cartesian_system cartesian_system_, vector position)
{
  matrix scale = build_scale_matrix(1.0),
    rotation = build_cartesian_system_matrix(cartesian_system_),
    translation = build_translation_matrix(position),
    scale_rotation = multiply_matrices(rotation, scale),
    transformation = multiply_matrices(translation, scale_rotation);
  return transformation;
}
vector apply_transformation_matrix(vector orig, const matrix _matrix)
{
  return build_vector(_matrix.ele[0][0] * orig.x + _matrix.ele[0][1] * orig.y +
		      _matrix.ele[0][2] * orig.z + _matrix.ele[0][3] * 1.0,
		      _matrix.ele[1][0] * orig.x + _matrix.ele[1][1] * orig.y +
		      _matrix.ele[1][2] * orig.z + _matrix.ele[1][3] * 1.0,
		      _matrix.ele[2][0] * orig.x + _matrix.ele[2][1] * orig.y +
		      _matrix.ele[2][2] * orig.z + _matrix.ele[2][3] * 1.0);
}
matrix build_cofactor_matrix(const matrix _matrix)
{
  unsigned char i, j, x, y;
  matrix cofactor, minor;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){
      for(y = 0; y < 3; y++){
	for(x = 0; x < 3; x++)
	  minor.ele[y][x] = _matrix.ele[(y>=i)?y+1:y][(x>=j)?x:x+1];
      }
      cofactor.ele[i][j] = determinant_helper(minor, 3) * ((i+j%2==1)?-1:1);
    }
  }
  return cofactor;
}

float matrix_determinant(const matrix _matrix)
{
  return determinant_helper(_matrix, 4);
}
static float determinant_helper(const matrix _matrix, char n)
{
  if(n < 2){
    return 0.0;
  }
  if(n == 2){
    return _matrix.ele[0][0] * _matrix.ele[1][1] -
      _matrix.ele[0][1] * _matrix.ele[1][0];
  }
  unsigned char i, x, y;
  float det = 0.0, m;
  matrix minor;
  for(i = 0; i < n; i++){
    m = _matrix.ele[0][i];
    for(y = 1; y < n; y++){
      for(x = 0; x < n; x++){
	if(x == i) continue;
	minor.ele[y - 1][(x > i)?x-1:x] = _matrix.ele[y][x];
      }
    }
    m *= (i % 2 == 0) ? 1 : -1;
    det += m  * determinant_helper(minor, n-1);
  }
  return det;
}
