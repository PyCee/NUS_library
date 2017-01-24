#ifndef NUS_FRUSTUM_H
#define NUS_FRUSTUM_H

#include "NUS_matrix.h"

typedef struct NUS_frustum{
  double vertical_fov, aspect_ratio, near, far;
  NUS_matrix clip_matrix;
} NUS_frustum;

NUS_frustum nus_frustum_build(double, double, double, double);
NUS_frustum nus_frustum_infinite(double, double, double);

#endif /* NUS_FRUSTUM_H */
