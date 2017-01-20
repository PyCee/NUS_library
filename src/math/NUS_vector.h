#ifndef NUS_VECTOR_H
#define NUS_VECTOR_H

typedef struct NUS_vector{
  double x, y, z;
} NUS_vector;

NUS_vector nus_vector_build(double, double, double);
NUS_vector nus_vector_add(NUS_vector, NUS_vector);
NUS_vector nus_vector_scale(NUS_vector, double);
NUS_vector nus_vector_subtract(NUS_vector, NUS_vector);
NUS_vector nus_vector_normalize(NUS_vector);
double nus_vector_dot(NUS_vector, NUS_vector);
NUS_vector nus_vector_cross(NUS_vector, NUS_vector);
NUS_vector nus_vector_interpolate(NUS_vector, NUS_vector, double);
double nus_vector_mag_sq(NUS_vector);
char nus_vector_cmp(NUS_vector, NUS_vector, double);
void nus_vector_print(NUS_vector);

#endif /* NUS_VECTOR_H */
