#include "NUS_store.h"
#include "../NUS_log.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

NUS_result nusm_store(char *file_path, NUS_binary_model *p_binary_model)
{
  FILE *output_file;
  char const magic_string_var[] = NUSM_MAGIC_STRING;
  
  if((output_file = fopen(file_path, "wb")) == NULL){
    NUS_LOG_ERROR("failed to open file \"%s\" for binary write\n", file_path);
    return NUS_FAILURE;
  }
  
  /* Output Magic String */
  fwrite(magic_string_var, strlen(NUSM_MAGIC_STRING), 1, output_file);
  
  // Output all sizes at once
  fwrite(p_binary_model, sizeof(p_binary_model->vertex_count),
	 NUS_BINARY_MODEL_SIZE_COUNT, output_file);
  
  /* Output Vertex Data */
  if(!fwrite(p_binary_model->vertices, p_binary_model->vertex_count *
	     sizeof(*p_binary_model->vertices), 1, output_file)){
    NUS_LOG_ERROR("failed to write vertex data\n");
    return NUS_FAILURE;
  }
  
  /* Output Index Data */
  if(!fwrite(p_binary_model->indices, p_binary_model->index_count *
	     sizeof(*p_binary_model->indices), 1, output_file)){
    NUS_LOG_ERROR("failed to write index data\n");
    return NUS_FAILURE;
  }
  
  /* Output Texture Data */
  if(p_binary_model->texture_data_size){
    // If texturing is supported
    if(!fwrite(p_binary_model->texture_data, p_binary_model->texture_data_size,
	       1, output_file)){
      // If texture data was not read
      NUS_LOG_ERROR("failed to write texture data\n");
      return NUS_FAILURE;
    }
  }

  /* Output Skeleton Data */
  if(!fwrite(&p_binary_model->skeleton.joint_count,
	     sizeof(p_binary_model->skeleton.joint_count), 1, output_file)){
    NUS_LOG_ERROR("failed to write skeleton joint count\n");
    return NUS_FAILURE;
  }
  if(p_binary_model->skeleton.joint_count){
    if(!fwrite(p_binary_model->skeleton.joints, p_binary_model->skeleton.joint_count *
	       sizeof(*p_binary_model->skeleton.joints), 1, output_file)){
      // If skeleton is supported, and data 
      NUS_LOG_ERROR("failed to write skeleton joints\n");
      return NUS_FAILURE;
    }
  }
  
  /* Output Animation Data  */
  for(uint32_t i = 0; i < p_binary_model->animation_count; ++i){
    if(!fwrite(&p_binary_model->animations[i].frame_count,
	       sizeof(p_binary_model->animations[i].frame_count), 1, output_file)){
      NUS_LOG_ERROR("failed to write animation %d keyframe count data\n", i);
      return NUS_FAILURE;
    }
    if(!fwrite(p_binary_model->animations[i].times,
	       sizeof(*p_binary_model->animations[i].times) *
	       p_binary_model->animations[i].frame_count, 1, output_file)){
      NUS_LOG_ERROR("failed to write animation %d keyframe time data\n", i);
      return NUS_FAILURE;
    }
    for(uint32_t j = 0; j < p_binary_model->animations[i].frame_count; ++j){
      if(!fwrite(p_binary_model->animations[i].keyframes[j].joints,
		 sizeof(*p_binary_model->animations[i].keyframes[j].joints) *
		 p_binary_model->skeleton.joint_count, 1, output_file)){
	NUS_LOG_ERROR("failed to write animation %d joint data\n", i);
	return NUS_FAILURE;
      }
    }
  }
  
  fclose(output_file);
  return NUS_SUCCESS;
}
