#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "matrix.h"

typedef struct frustum{
  float vertical_fov_rad, aspect_ratio, near, far;
  matrix clip_matrix;
} frustum;

void init_frustum(void);

frustum gen_frustum(void);
void free_frustum(frustum *);
frustum build_frustum(float, float, float, float);
frustum build_infinite_frustum(float, float, float);

#endif /* _FRUSTUM_H_ */
