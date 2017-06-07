#ifndef NUS_COMPONENT_KEY_H
#define NUS_COMPONENT_KEY_H

#include "NUS_bool.h"

#define NUS_COMPONENT_KEY_HEALTH 1

/* Physics */
#define NUS_COMPONENT_KEY_POSITION (NUS_COMPONENT_KEY_HEALTH << 1)
#define NUS_COMPONENT_KEY_VELOCITY (NUS_COMPONENT_KEY_POSITION << 1)
#define NUS_COMPONENT_KEY_COLLIDABLE (NUS_COMPONENT_KEY_VELOCITY << 1)

/* Render */
#define NUS_COMPONENT_KEY_RENDER (NUS_COMPONENT_KEY_COLLIDABLE << 1)

typedef unsigned int NUS_component_key;

NUS_bool nus_component_key_unlock(NUS_component_key, NUS_component_key);

#endif /* NUS_COMPONENT_KEY_H */
