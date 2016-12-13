#ifndef _VECTOR_H_
#define _VECTOR_H_

#define DEFAULT_VECTOR {0.0, 0.0, 0.0}

/* alignment : 36 bits */
typedef struct vector{
  float x, y, z;
} vector;

vector gen_vector(void);
vector build_vector(float, float, float);

float mag_sq_vector(vector);
vector normalize_vector(vector);

vector add_vector(vector, vector);
vector sub_vector(vector, vector);
unsigned char cmp_vector(vector, vector);
vector scale_vector(vector, double);
float dot_vector(vector, vector);
vector cross_vector(vector, vector);
vector interpolate_vector(vector, vector, float);
void print_vector(vector);

float point_line_sq_dist(vector, vector, vector);
float line_line_sq_dist(vector, vector, vector, vector);
float closest_point_line(vector, vector, vector);
float *closest_point_lines(vector, vector, vector, vector);

#endif /* _VECTOR_H_ */
