#ifndef NUS_KINEMATIC_PROPERTY_H
#define NUS_KINEMATIC_PROPERTY_H

#include "../math/NUS_vector.h"

typedef struct NUS_kinematic_factor{
  NUS_vector effect;
  unsigned int id;
} NUS_kinematic_factor;

typedef struct NUS_kinematic_property{
  NUS_kinematic_factor *factors;
  NUS_vector value;
  unsigned int factor_count;
} NUS_kinematic_property;

NUS_kinematic_property nus_kinematic_property_build(void);
void nus_kinematic_property_free(NUS_kinematic_property *);
NUS_vector nus_kinematic_property_integral(NUS_kinematic_property *, double);
void nus_kinematic_property_set_factor
(NUS_kinematic_property *, unsigned int, NUS_vector);
unsigned int nus_kinematic_property_append_factor(NUS_kinematic_property *, NUS_vector);
void nus_kinematic_property_remove(NUS_kinematic_property *, unsigned int);
void nus_kinematic_property_impulse(NUS_kinematic_property *, NUS_vector);
void nus_kinematic_property_print(NUS_kinematic_property);


#endif /* NUS_KINEMATIC_PROPERTY_H */
