#ifndef NUS_MATRIX_H
#define NUS_MATRIX_H

struct NUS_vector;
struct NUS_axes;
struct NUS_quaternion;

typedef struct NUS_matrix{
  double ele[4][4];
} NUS_matrix;

NUS_matrix nus_matrix_build(double, double, double, double,
			    double, double, double, double,
			    double, double, double, double,
			    double, double, double, double);
NUS_matrix nus_matrix_identity(void);
NUS_matrix nus_matrix_zero(void);
NUS_matrix nus_matrix_transpose(NUS_matrix);
NUS_matrix nus_matrix_scale(NUS_matrix, double);
NUS_matrix nus_matrix_multiply(NUS_matrix, NUS_matrix);

NUS_matrix nus_matrix_translation(struct NUS_vector);
NUS_matrix nus_matrix_rotation(struct NUS_axes);

NUS_matrix nus_matrix_transformation
(struct NUS_vector, struct NUS_axes);

NUS_matrix nus_matrix_inverted(NUS_matrix);
void nus_matrix_print(NUS_matrix);

#endif /* NUS_MATRIX_H */
