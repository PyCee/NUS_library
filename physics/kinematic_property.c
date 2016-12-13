#include "kinematic_property.h"
#include <stdlib.h>
#include <stdio.h>

kinematic_factor gen_kinematic_factor(void)
{
  static kinematic_factor kinematic_factor_ = DEFAULT_KINEMATIC_FACTOR;
  return kinematic_factor_;
}
void free_kinematic_factor(kinematic_factor *kinematic_factor_)
{
  *kinematic_factor_ = gen_kinematic_factor();
}
kinematic_factor build_kinematic_factor(vector value, float scale, short id)
{
  kinematic_factor kinematic_factor_ = gen_kinematic_factor();
  kinematic_factor_.value = value;
  kinematic_factor_.scale = scale;
  kinematic_factor_.id = id;
  return kinematic_factor_;
}
void print_kinematic_factor(kinematic_factor kinematic_factor_)
{
  printf("kinematic_factor index %d has scale %f and value:\n",
	 kinematic_factor_.id,
	 kinematic_factor_.scale);
  print_vector(kinematic_factor_.value);
}

static short get_kinematic_property_index_from_id(kinematic_property, short);
static short get_open_kinematic_factor_id(kinematic_property);

kinematic_property gen_kinematic_property(void)
{
  static kinematic_property kinematic_property_ = DEFAULT_KINEMATIC_PROPERTY;
  return kinematic_property_;
}
void free_kinematic_property(kinematic_property *kinematic_property_)
{
  if(kinematic_property_->factors){
    short i;
    for(i = 0; i < kinematic_property_->num_factors; i++){
      free_kinematic_factor(kinematic_property_->factors + i);
    }
    free(kinematic_property_->factors);
    kinematic_property_->factors = NULL;
  }
  kinematic_property_->num_factors = 0;
}
vector update_kinematic_property
(kinematic_property *kinematic_property_, double delta_ms)
{
  short i;
  vector integral = kinematic_property_->value;
  for(i = 0; i < kinematic_property_->num_factors; i++){
    integral = add_vector(integral, scale_vector(kinematic_property_->factors[i].value,
					 kinematic_property_->factors[i].scale));
  }
  return scale_vector(integral, delta_ms);
}

void replace_kinematic_factor
(kinematic_property *kinematic_property_, vector factor, float scale, short id)
{
  short index = get_kinematic_property_index_from_id(*kinematic_property_, id);
  if(index != -1){
    kinematic_property_->factors[index] = build_kinematic_factor(factor, scale, id);
  } else {
    printf("Attempting to replace invalid kinematic_factor id\n");
  }
}
short add_kinematic_factor
(kinematic_property *kinematic_property_, vector factor, float scale)
{
  short id = get_open_kinematic_factor_id(*kinematic_property_);
  kinematic_property_->factors = realloc(kinematic_property_->factors,
					 sizeof(*kinematic_property_->factors) *
					 (++kinematic_property_->num_factors));
  kinematic_property_->factors[kinematic_property_->num_factors - 1] =
    build_kinematic_factor(factor, scale, id);
  return id;
}
void remove_kinematic_factor
(kinematic_property *kinematic_property_, short id)
{
  short index = get_kinematic_property_index_from_id(*kinematic_property_, id);
  if(index == -1){
    return;
  }
  else if(kinematic_property_->num_factors == 1){
    free_kinematic_property(kinematic_property_);
  }
  else{
    kinematic_factor *outdated_factors = kinematic_property_->factors;
    short i;
    kinematic_property_->factors = malloc(sizeof(*kinematic_property_->factors) *
					  --kinematic_property_->num_factors);
    for(i = 0; i < index; ++i){
      kinematic_property_->factors[i] = outdated_factors[i];
    }
    free_kinematic_factor(kinematic_property_->factors + i);
    for(++i; i < kinematic_property_->num_factors; ++i){
      kinematic_property_->factors[i - 1] = outdated_factors[i];
    }
    free(outdated_factors);
  }
}
void add_instant_kinematic_factor
(kinematic_property *kinematic_property_, vector instant)
{
  kinematic_property_->value = add_vector(kinematic_property_->value, instant);
}
void print_kinematic_property(kinematic_property kinematic_property_)
{
  print_vector(kinematic_property_.value);
  short i;
  for(i = 0; i < kinematic_property_.num_factors; ++i){
    print_kinematic_factor(kinematic_property_.factors[i]);
  }
}
static short get_kinematic_property_index_from_id
(kinematic_property kinematic_property_, short index)
{
  short i;
  for(i = 0; i < kinematic_property_.num_factors; i++){
    if(kinematic_property_.factors[i].id == index)
      return i;
  }
  return -1;
}
static short get_open_kinematic_factor_id
(kinematic_property kinematic_property_)
{
  short i;
  char *index_used = calloc(1, kinematic_property_.num_factors);
  for(i = 0; i < kinematic_property_.num_factors; i++){
    if(kinematic_property_.factors[i].id < kinematic_property_.num_factors)
      index_used[i] = 1;
  }
  for(i = 0; i < kinematic_property_.num_factors; i++){
    if(!index_used[i]){
      break;
    }
  }
  free(index_used);
  return i;
}
