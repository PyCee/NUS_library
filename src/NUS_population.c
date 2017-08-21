#include "NUS_population.h"
#include <stdlib.h>

NUS_population nus_population_build(void)
{
  NUS_population h_population = malloc(sizeof(NUS_REC_HANDLE(h_population)));
  NUS_REC_HANDLE(h_population).entities = NULL;
  NUS_REC_HANDLE(h_population).entity_count = 0;
  return h_population;
}
void nus_population_free(NUS_population h_population)
{
  free(NUS_REC_HANDLE(h_population).entities);
  free(h_population);
}
void nus_population_add(NUS_population h_population, NUS_entity h_entity)
{
  if(nus_population_contains_entity(h_population, h_entity))
    return;
  NUS_REC_HANDLE(h_population).entities =
    realloc(NUS_REC_HANDLE(h_population).entities,
	    sizeof(*NUS_REC_HANDLE(h_population).entities) *
	    ++NUS_REC_HANDLE(h_population).entity_count);
  NUS_REC_HANDLE(h_population).entities
    [NUS_REC_HANDLE(h_population).entity_count - 1] = h_entity;
}
void nus_population_remove(NUS_population h_population, NUS_entity h_entity)
{
  if(!nus_population_contains_entity(h_population, h_entity))
    return;
  NUS_entity *ph_tmp_entities = NUS_REC_HANDLE(h_population).entities;
  NUS_REC_HANDLE(h_population).entities =
    malloc(sizeof(*NUS_REC_HANDLE(h_population).entities) *
	   --NUS_REC_HANDLE(h_population).entity_count);
  for(int i = 0; i < NUS_REC_HANDLE(h_population).entity_count + 1; ++i){
    if(ph_tmp_entities[i] == h_entity)
      continue;
    *NUS_REC_HANDLE(h_population).entities = ph_tmp_entities[i];
    ++NUS_REC_HANDLE(h_population).entities;
  }
  NUS_REC_HANDLE(h_population).entities -=
    NUS_REC_HANDLE(h_population).entity_count;
}
NUS_bool nus_population_contains_entity
(NUS_population h_population, NUS_entity h_entity)
{
  int i;
  for(i = 0; i < NUS_REC_HANDLE(h_population).entity_count; ++i){
    if(NUS_REC_HANDLE(h_population).entities[i] == h_entity)
      return NUS_TRUE;
  }
  return NUS_FALSE;
}
