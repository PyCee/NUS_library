#ifndef NUS_MOVEMENT_H
#define NUS_MOVEMENT_H

#include "NUS_kinematic_property.h"
#include "../math/NUS_vector.h"

#define NUS_GRAVITY_FORCE 9.8

struct NUS_physics_state;

typedef struct NUS_movement{
  NUS_kinematic_property momentum, force;
  NUS_vector displacement;
  unsigned int gravity_id,
    mass;
} NUS_movement;

NUS_movement nus_movement_build(void);
void nus_movement_free(NUS_movement *);
struct NUS_physics_state nus_movement_apply
(NUS_movement *, struct NUS_physics_state);
void nus_movement_set_gravity(NUS_movement *, NUS_vector);
void nus_movement_set_mass(NUS_movement *, unsigned int);
void nus_movement_print(NUS_movement);

#endif /* NUS_MOVEMENT_H */
