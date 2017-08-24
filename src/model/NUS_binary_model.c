#include "NUS_binary_model.h"
#include "../NUS_log.h"

void nus_binary_model_free(NUS_binary_model *p_binary_model)
{
  free(p_binary_model->vertices);
  free(p_binary_model->indices);
  if(p_binary_model->texture_data_size){
    free(p_binary_model->texture_data);
  }
  nus_skeleton_free(&p_binary_model->skeleton);
  if(p_binary_model->animation_count){
    for(int i = 0; i < p_binary_model->animation_count; ++i){
      nus_animation_free(p_binary_model->animations + i);
    }
    free(p_binary_model->animations);
  }
}
