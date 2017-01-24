#include "NUS_frustum.h"
#include <math.h>

NUS_frustum nus_frustum_build
(double vertical_fov, double aspect_ratio, double near, double far)
{
  double cot_half_fov = 1.0 / tan(vertical_fov / 2.0);
  return (NUS_frustum){ .vertical_fov = vertical_fov,
      .aspect_ratio = aspect_ratio,
      .near = near,
      .far = far,
      .clip_matrix = nus_matrix_build(cot_half_fov / aspect_ratio, 0.0, 0.0, 0.0,
				      0.0, cot_half_fov, 0.0, 0.0,
				      0.0, 0.0, -1.0 * (far + near) / (far - near),
				      -2.0 * far * near / (far - near),
				      0.0, 0.0, -1.0, 0.0)
      };
}
NUS_frustum nus_frustum_infinite
(double vertical_fov, double aspect_ratio, double near)
{
  double cot_half_fov = 1.0 / tan(vertical_fov / 2.0);
  return (NUS_frustum){ .vertical_fov = vertical_fov,
      .aspect_ratio = aspect_ratio,
      .near = near,
      .far = 0.0,
      .clip_matrix = nus_matrix_build(cot_half_fov / aspect_ratio, 0.0, 0.0, 0.0,
				      0.0, cot_half_fov, 0.0, 0.0,
				      0.0, 0.0, -1.0, -2.0 * near,
				      0.0, 0.0, -1.0, 0.0)
      };
}
