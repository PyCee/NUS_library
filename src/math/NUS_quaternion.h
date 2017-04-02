#ifndef NUS_QUATERNION_H
#define NUS_QUATERNION_H

struct NUS_vector;

typedef struct NUS_quaternion{
  double w, x, y, z;
} NUS_quaternion;

NUS_quaternion nus_quaternion_build(double, double, double, double);
NUS_quaternion nus_quaternion_pure(struct NUS_vector);
NUS_quaternion nus_quaternion_unit(struct NUS_vector, double);
NUS_quaternion nus_quaternion_conjugate(NUS_quaternion);
NUS_quaternion nus_quaternion_h_product(NUS_quaternion,  NUS_quaternion);
struct NUS_vector nus_quaternion_apply_rotation(NUS_quaternion, struct NUS_vector);
NUS_quaternion nus_quaternion_lerp(NUS_quaternion, NUS_quaternion, double);
NUS_quaternion nus_quaternion_slerp(NUS_quaternion, NUS_quaternion, double);
NUS_quaternion nus_quaternion_normalize(NUS_quaternion);
void nus_quaternion_print(NUS_quaternion);

#endif /* NUS_QUATERNION_H */
