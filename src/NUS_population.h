#ifndef NUS_POPULATION_H
#define NUS_POPULATION_H

#include "NUS_handle.h"
#include "NUS_entity.h"
#include "NUS_bool.h"

typedef struct NUS_population_t{
  NUS_entity *entities;
  unsigned int entity_count;
} NUS_population_t;

NUS_DEFINE_HANDLE(NUS_population);

NUS_population nus_population_build(void);
void nus_population_free(NUS_population);
void nus_population_add(NUS_population, NUS_entity);
void nus_population_remove(NUS_population, NUS_entity);
NUS_bool nus_population_contains_entity(NUS_population, NUS_entity);

#endif /* NUS_POPULATION_H */
