#include "frustum.h"
#include "matrix.h"
#include <stdlib.h>
#include <GL/glew.h>
#include <math.h>
frustum gen_frustum(void)
{
  frustum _frustum;
  _frustum.vertical_fov_rad = 0.0;
  _frustum.aspect_ratio = 0.0;
  _frustum.near = 0.1;
  _frustum.far = 1.0;
  _frustum.clip_matrix = build_identity_matrix();
  return _frustum;
}
void free_frustum(frustum *fru)
{
}
frustum build_frustum
(float fov_y_rad, float aspect_ratio, float near, float far)
{
  frustum _frustum = gen_frustum();
  _frustum.vertical_fov_rad = fov_y_rad;
  _frustum.aspect_ratio = aspect_ratio;
  _frustum.near = near;
  _frustum.far = far;

  float cot_half_fov = 1.0 / tan(fov_y_rad / 2);
  _frustum.clip_matrix.ele[0][0] = cot_half_fov / aspect_ratio;
  _frustum.clip_matrix.ele[1][1] = cot_half_fov;
  _frustum.clip_matrix.ele[2][2] = -1.0 * (far + near) / (far - near);
  _frustum.clip_matrix.ele[2][3] = -2.0 * far * near / (far - near);
  _frustum.clip_matrix.ele[3][2] = -1;
  return _frustum;
}
frustum build_infinite_frustum
(float vertical_fov_rad, float aspect_ratio, float near)
{
  frustum _frustum = build_frustum(vertical_fov_rad, aspect_ratio, near, near + 1.0);
  _frustum.far = 0;
  _frustum.clip_matrix.ele[2][2] = -1.0;
  _frustum.clip_matrix.ele[2][3] = -1.0 * 2 * near;
  return _frustum;
}
