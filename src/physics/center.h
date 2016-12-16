#ifndef _CENTER_H_
#define _CENTER_H_

#include "physics_state.h"
#include "movement.h"
#include "orientation.h"
#include <stdlib.h>

#define DEFAULT_CENTER {DEFAULT_MOVEMENT, DEFAULT_ORIENTATION, DEFAULT_PHYSICS_STATE, \
			DEFAULT_PHYSICS_STATE, DEFAULT_PHYSICS_STATE, DEFAULT_MATRIX}

/* alignment : 36 bits */
typedef struct center{
  movement move;
  orientation orient;
  physics_state prev_state, next_state, curr_state;
  matrix world_transformation;
} center;

center gen_center(void);
void free_center(center *);
void batch_update_center(void *, size_t, size_t, short, double);
void set_mass(center *, float);
void set_directional_gravity(center *, vector);
void print_center(center);

#endif /* _CENTER_H_ */
