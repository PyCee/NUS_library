#ifndef _MATRIX_H_
#define _MATRIX_H_

#define DEFAULT_MATRIX {{{0.0, 0.0, 0.0, 0.0}, \
			 {0.0, 0.0, 0.0, 0.0}, \
			 {0.0, 0.0, 0.0, 0.0}, \
			 {0.0, 0.0, 0.0, 0.0}}}

struct vector;
struct quaternion;
struct cartesian_system;

/* alignment : 36 bits */
typedef struct matrix{
  float ele[4][4];
} matrix;
matrix gen_matrix(void);
void reset_matrix(matrix *, float *);
matrix build_matrix(float, float, float, float, float, float, float, float,
		    float, float, float, float, float, float, float, float);
matrix build_identity_matrix(void);
matrix build_zero_matrix(void);
matrix build_inverted_matrix(const matrix);
matrix scale_matrix(matrix, float);
matrix build_scale_matrix(float);
matrix build_transposed_matrix(const matrix);
matrix multiply_matrices(const matrix, const matrix);
void print_matrix(const matrix);
matrix build_quaternion_matrix(struct quaternion);
matrix build_cartesian_system_matrix(struct cartesian_system);
matrix build_translation_matrix(struct vector);
matrix build_transformation_matrix(struct cartesian_system, struct vector);
struct vector apply_transformation_matrix(struct vector, const matrix);
float matrix_determinant(const matrix);
matrix build_cofactor_matrix(const matrix);
#endif /* _MATRIX_H_ */
