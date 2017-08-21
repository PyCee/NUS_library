#include "NUS_binary_model.h"

void nus_binary_model_free(NUS_binary_model *p_binary_model)
{
  free(p_binary_model->vertex_data);
  free(p_binary_model->index_data);
  free(p_binary_model->texture_data);
  free(p_binary_model->skeleton_data);
  for(int i = 0; i < p_binary_model->animation_count; ++i){
    free(p_binary_model->p_animations[i].p_keyframe_joints);
    free(p_binary_model->p_animations[i].p_times);
  }
  free(p_binary_model->p_animations);
}
