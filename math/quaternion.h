#ifndef _QUANTERNION_H_
#define _QUANTERNION_H_

struct vector;
struct mat4;

typedef struct quaternion{
	float w, x, y, z;
} quaternion;

quaternion gen_quaternion(void);
void free_quaternion(quaternion *);
void print_quaternion(quaternion);
quaternion build_pure_quaternion(struct vector);
quaternion build_unit_quaternion(struct vector, float);
quaternion conjugate_quaternion(quaternion);
quaternion h_product_quaternion(quaternion, quaternion);

struct vector apply_rotation_quaternion(struct vector, quaternion);
quaternion lerp_quaternion(quaternion, quaternion, float);
quaternion slerp_quaternion(quaternion, quaternion, float);

#endif /* _QUANTERNION_H_ */
