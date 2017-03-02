#include "NUS_kinematic_property.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

static unsigned int nus_kinematic_property_get_factor_index
(NUS_kinematic_property, unsigned int);
static unsigned int nus_kinematic_property_next_id
(NUS_kinematic_property);


NUS_kinematic_property nus_kinematic_property_build(void)
{
  return (NUS_kinematic_property){
    .factors = NULL,
      .value = nus_vector_build(0.0, 0.0, 0.0),
      .factor_count = 0
  };
}
void nus_kinematic_property_free(NUS_kinematic_property *p_kinematic_property)
{
  if(p_kinematic_property->factors != NULL){
    free(p_kinematic_property->factors);
    p_kinematic_property->factors = NULL;
    p_kinematic_property->factor_count = 0;
  }
  p_kinematic_property->value = nus_vector_build(0.0, 0.0, 0.0);
}
NUS_vector nus_kinematic_property_integral
(NUS_kinematic_property *p_kinematic_property, double delta_s)
{
  unsigned int i;
  NUS_vector delta = p_kinematic_property->value;
  for(i = 0; i < p_kinematic_property->factor_count; ++i){
    delta = nus_vector_add(delta, p_kinematic_property->factors[i].effect);
  }
  return nus_vector_scale(delta, delta_s);
}
void nus_kinematic_property_set_factor
(NUS_kinematic_property *p_kinematic_property, unsigned int id, NUS_vector vector)
{
  unsigned int index =
    nus_kinematic_property_get_factor_index(*p_kinematic_property, id);
  p_kinematic_property->factors[index].effect = vector;
}
unsigned int nus_kinematic_property_append_factor
(NUS_kinematic_property *p_kinematic_property, NUS_vector vector)
{
  unsigned int next_id = nus_kinematic_property_next_id(*p_kinematic_property),
    next_count = ++p_kinematic_property->factor_count;
  p_kinematic_property->factors = realloc(p_kinematic_property->factors,
					  sizeof(*p_kinematic_property->factors) *
					  next_count);
  p_kinematic_property->factors[next_count - 1] = (NUS_kinematic_factor){
    .effect = vector,
    .id = next_id
  };
  printf("effect: ");
  nus_vector_print(p_kinematic_property->factors[next_count-1].effect);
  return next_id;
}
void nus_kinematic_property_remove
(NUS_kinematic_property *p_kinematic_property, unsigned int id)
{
  if(p_kinematic_property->factor_count == 1){
    p_kinematic_property->factor_count = 0;
    free(p_kinematic_property->factors);
    p_kinematic_property->factors = NULL;
  }
  unsigned int index = nus_kinematic_property_get_factor_index(*p_kinematic_property,
							       id);
  NUS_kinematic_factor *new_factors = malloc(sizeof(*new_factors) *
					     p_kinematic_property->factor_count);
  unsigned int i;
  for(i = 0; i < index; ++i){
    new_factors[i] = p_kinematic_property->factors[i];
  }
  for(++i; i < p_kinematic_property->factor_count; ++i){
    new_factors[i - 1] = p_kinematic_property->factors[i];
  }
  free(p_kinematic_property->factors);
  p_kinematic_property->factors = new_factors;
  --p_kinematic_property->factor_count;
}
void nus_kinematic_property_impulse
(NUS_kinematic_property *p_kinematic_property, NUS_vector impulse)
{
  p_kinematic_property->value = nus_vector_add(p_kinematic_property->value, impulse);
}
void nus_kinematic_property_print(NUS_kinematic_property kinematic_property)
{
  printf("printing kinematic property with %d elements\n",
	 kinematic_property.factor_count);
  unsigned int i;
  for(i = 0; i < kinematic_property.factor_count; ++i){
    printf("factor %d has id %d and effect ", i, kinematic_property.factors[i].id);
    nus_vector_print(kinematic_property.factors[i].effect);
  }
}
static unsigned int nus_kinematic_property_get_factor_index
(NUS_kinematic_property kinematic_property, unsigned int id)
{
  unsigned int i;
  for(i = 0; i < kinematic_property.factor_count; ++i){
    if(id == kinematic_property.factors[i].id){
      return i;
    }
  }
  printf("ERROR::could not resolve kinematic factor id %d::id doesn't exist\n", id);
  return UINT_MAX;
}
static unsigned int nus_kinematic_property_next_id
(NUS_kinematic_property kinematic_property)
{
  unsigned int i;
  char id_available[kinematic_property.factor_count];
  memset(id_available, 1, kinematic_property.factor_count);
  for(i = 0; i < kinematic_property.factor_count; ++i){
    if(kinematic_property.factors[i].id >= kinematic_property.factor_count){
      continue;
    }
    id_available[i] = 0;
  }
  for(i = 0; i < kinematic_property.factor_count; ++i){
    if(id_available[i]){
      break;
    }
  }
  return i;
}
