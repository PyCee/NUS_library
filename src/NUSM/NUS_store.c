#include "NUS_store.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

NUS_result nusm_store(char *file_path, NUS_binary_model *p_binary_model)
{
  FILE *output_file;
  char const magic_string_var[] = NUSM_MAGIC_STRING;
  
  if((output_file = fopen(file_path, "wb")) == NULL){
    printf("NUSM_ERROR::failed to open file \"%s\" for binary write\n", file_path);
    return NUS_FAILURE;
  }
  
  /* Output Magic String */
  fwrite(magic_string_var, strlen(NUSM_MAGIC_STRING), 1, output_file);
  
  // Output all sizes at once
  fwrite(p_binary_model, sizeof(size_t), NUS_BINARY_MODEL_SIZE_COUNT, output_file);
  
  /* Output Vertex Data */
  if(!fwrite(p_binary_model->vertex_data, p_binary_model->vertex_data_size,
	     1, output_file)){
    printf("NUSM::ERROR::failed to write vertex data\n");
    return NUS_FAILURE;
  }
  
  /* Output Index Data */
  if(!fwrite(p_binary_model->index_data, p_binary_model->index_data_size,
	     1, output_file)){
    printf("NUSM::ERROR::failed to write index data\n");
    return NUS_FAILURE;
  }
  
  /* Output Texture Data */
  if(!fwrite(p_binary_model->texture_data, p_binary_model->texture_data_size,
	    1, output_file)){
    printf("NUSM::ERROR::failed to write texture data\n");
    return NUS_FAILURE;
  }

  /* Output Skeleton Data */
  if(!fwrite(p_binary_model->skeleton_data, p_binary_model->skeleton_data_size,
	    1, output_file)){
    printf("NUSM::ERROR::failed to write skeleton data\n");
    return NUS_FAILURE;
  }
  
  /* Output Animation Data  */
  for(size_t i = 0; i < p_binary_model->animation_count; ++i){
    if(!fwrite(&p_binary_model->p_animations[i].keyframe_count, sizeof(unsigned int),
	       1, output_file)){
      printf("NUSM::ERROR::failed to write animation %zu keyframe count data\n", i);
      return NUS_FAILURE;
    }
    if(!fwrite(p_binary_model->p_animations[i].p_keyframe_joints,
	       sizeof(*p_binary_model->p_animations[i].p_keyframe_joints) *
	       p_binary_model->p_animations[i].keyframe_count *
	       p_binary_model->joint_count, 1, output_file)){
      printf("NUSM::ERROR::failed to write animation %zu joint data\n", i);
      return NUS_FAILURE;
    }
    
    if(!fwrite(p_binary_model->p_animations[i].p_times,
	       sizeof(*p_binary_model->p_animations[i].p_times) *
	       p_binary_model->p_animations[i].keyframe_count, 1, output_file)){
      printf("NUSM::ERROR::failed to write animation %zu keyframe time data\n", i);
      return NUS_FAILURE;
    }
  }
  
  fclose(output_file);
  return NUS_SUCCESS;
}
