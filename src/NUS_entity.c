#include "NUS_entity.h"
#include <stdlib.h>

NUS_entity nus_entity_build(NUS_component_key key)
{
  NUS_entity h_entity = malloc(sizeof(NUS_REC_HANDLE(h_entity)));
  NUS_REC_HANDLE(h_entity).key = key;
  return h_entity;
}
void nus_entity_free(NUS_entity h_entity)
{
  free(h_entity);
}
