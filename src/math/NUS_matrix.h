#ifndef NUS_MATRIX_H
#define NUS_MATRIX_H

#include "../NUS_bool.h"

struct NUS_vector;
struct NUS_axes;
struct NUS_quaternion;

typedef struct NUS_matrix{
  float ele[4][4];
} NUS_matrix;

NUS_matrix nus_matrix_build(float, float, float, float,
			    float, float, float, float,
			    float, float, float, float,
			    float, float, float, float);
NUS_matrix nus_matrix_identity(void);
NUS_matrix nus_matrix_zero(void);
NUS_matrix nus_matrix_transpose(NUS_matrix);
NUS_matrix nus_matrix_multiply(NUS_matrix, NUS_matrix);

NUS_matrix nus_matrix_build_translation(struct NUS_vector);
NUS_matrix nus_matrix_build_scale(struct NUS_vector);
NUS_matrix nus_matrix_build_rotation(struct NUS_axes);

NUS_matrix nus_matrix_build_transformation
(struct NUS_vector, struct NUS_vector, struct NUS_axes);
struct NUS_vector nus_matrix_transform(NUS_matrix, struct NUS_vector);

NUS_matrix nus_matrix_inverse(NUS_matrix);
NUS_bool nus_matrix_cmp(NUS_matrix, NUS_matrix, float);
void nus_matrix_print(NUS_matrix);

#endif /* NUS_MATRIX_H */
