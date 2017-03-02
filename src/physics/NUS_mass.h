#ifndef NUS_MASS_H
#define NUS_MASS_H

#include "../math/NUS_matrix.h"
#include "NUS_physics_state.h"
#include "NUS_movement.h"
#include "NUS_orientation.h"
#include "../NUS_component_key.h"
#include <stdlib.h>

typedef struct NUS_mass{
  NUS_movement *movement;
  NUS_orientation *orientation;
  NUS_physics_state prev_state, next_state, curr_state;
  NUS_matrix world_transformation;
} NUS_mass;

NUS_mass nus_mass_build(NUS_component_key);
void nus_mass_free(NUS_mass *);
void nus_mass_batch_update(void *, size_t, size_t, unsigned int, double);
void nus_mass_print(NUS_mass);

#endif /* NUS_MASS_H */
