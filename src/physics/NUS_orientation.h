#ifndef NUS_ORIENTATION_H
#define NUS_ORIENTATION_H

#include "../math/NUS_vector.h"

struct NUS_physics_state;

typedef enum{
  NUS_ORIENTATION_MODE_FREE,
  NUS_ORIENTATION_MODE_LOCK,
  NUS_ORIENTATION_MODE_PAN
} NUS_orientation_mode;

typedef struct NUS_orientation{
  NUS_vector target;
  NUS_orientation_mode mode;
} NUS_orientation;

NUS_orientation nus_orientation_build(void);
void nus_orientation_free(NUS_orientation *);
struct NUS_physics_state nus_orientation_apply
(NUS_orientation *, struct NUS_physics_state);
void nus_orientation_print(NUS_orientation);

#endif /* NUS_ORIENTATION_H */
