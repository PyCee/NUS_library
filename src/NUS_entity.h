#ifndef NUS_ENTITY_H
#define NUS_ENTITY_H

#include "NUS_key.h"

typedef struct NUS_entity{
  unsigned int id;
  NUS_component_key key;
} NUS_entity;

#endif /* NUS_ENTITY_H */
