#ifndef _ORIENTATION_H_
#define _ORIENTATION_H_

#define DEFAULT_ORIENTATION {}

typedef struct orientation{
  /* Class that handles change in orientation of physics_state */
  
} orientation;

orientation gen_orientation(void);
void free_orientation(orientation *);
struct physics_state update_physics_state_orientation
(struct physics_state, orientation *);

#endif /* _ORIENTATION_H_ */
