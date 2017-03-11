#include "NUS_model.h"
#include <stdlib.h>

NUS_result nus_model_build(char const * const model_file, NUS_model *p_model)
{
  unsigned int i;
  /*
  p_model->vertex_count =
  p_model->vertices = malloc(sizeof(*p_model->vertices) * p_model->vertex_count);
  
  
  p_model->index_count = 
  p_model->indices = malloc(sizeof(*p_model->indices) * p_model->index_count)
  */
  for(i = 0; i < p_model->vertex_count; ++i){
    /* position */
    p_model->vertices[i].attributes[0] = 0.0;
    p_model->vertices[i].attributes[1] = 0.0;
    p_model->vertices[i].attributes[2] = 0.0;
    /* normal */
    p_model->vertices[i].attributes[3] = 0.0;
    p_model->vertices[i].attributes[4] = 0.0;
    p_model->vertices[i].attributes[5] = 0.0;
    /* tex_coords */
    p_model->vertices[i].attributes[6] = 0.0;
    p_model->vertices[i].attributes[7] = 0.0;
  }
  /*
    set indices
  */  
  return NUS_SUCCESS;
}
void nus_model_free(NUS_model *p_model)
{
  
}
