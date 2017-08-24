#include "NUS_validate.h"
#include "NUS_store.h"
#include "NUS_read.h"
#include "../NUS_log.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

NUS_result nusm_validate
(char *file_path, NUS_binary_model *p_binary_model)
{
  NUS_binary_model read_model;
  uint32_t i,
    j;

  // Read data from file
  if(nusm_read(file_path, &read_model) == 0){
    printf("NUSM_ERROR::failed to read nusm file\n");
    return NUS_FAILURE;
  }

  // Compare sizes
  if(memcmp(&read_model.vertex_count, &p_binary_model->vertex_count,
	    sizeof(read_model.vertex_count) * NUS_BINARY_MODEL_SIZE_COUNT)){
    NUS_LOG_ERROR("failed to validate nusm sizes\n");
    return NUS_FAILURE;
  }
  
  // Validate Vertex Data
  if(memcmp(read_model.vertices, p_binary_model->vertices,
	     sizeof(*read_model.vertices) * read_model.vertex_count)){
    NUS_LOG_ERROR("failed to validate vertex data\n");
    return NUS_FAILURE;
  }
  
  // Validate Index Data 
  if(memcmp(read_model.indices, p_binary_model->indices,
	     sizeof(*read_model.indices) * read_model.index_count)){
    NUS_LOG_ERROR("failed to validate index data\n");
    return NUS_FAILURE;
  }
  
  // Validate Texture Data
  if(memcmp(read_model.texture_data, p_binary_model->texture_data,
	     read_model.texture_data_size)){
    NUS_LOG_ERROR("failed to validate texture data\n");
      return NUS_FAILURE;
  }

  // Validate Skeleton Data 
  if(read_model.skeleton.joint_count != p_binary_model->skeleton.joint_count){
    NUS_LOG_ERROR("failed to validate skeleton joint count\n");
    return NUS_FAILURE;
  }
  if(memcmp(read_model.skeleton.joints, p_binary_model->skeleton.joints,
	     sizeof(*read_model.skeleton.joints) * read_model.skeleton.joint_count)){
    NUS_LOG_ERROR("failed to validate skeleton joints\n");
    return NUS_FAILURE;
  }
  
  // Validate Animation Data 
  for(i = 0; i < p_binary_model->animation_count; ++i){
    // For each animation
    
    if(read_model.animations[i].frame_count !=
       p_binary_model->animations[i].frame_count){
      NUS_LOG_ERROR("failed to validate animation keyframe count\n");
      return NUS_FAILURE;
    }
    if(memcmp(read_model.animations[i].times, p_binary_model->animations[i].times,
	       sizeof(*read_model.animations[i].times) *
	       read_model.animations[i].frame_count)){
      NUS_LOG_ERROR("failed to validate animation frame times\n");
      return NUS_FAILURE;
    }
    for(j = 0; j < p_binary_model->animations[i].frame_count; ++j){
      // For each frame
      
      if(memcmp(read_model.animations[i].keyframes[j].joints,
		p_binary_model->animations[i].keyframes[j].joints,
		sizeof(*read_model.animations[i].keyframes[j].joints) *
		read_model.skeleton.joint_count)){
	NUS_LOG_ERROR("failed to validate animation joint data\n");
	return NUS_FAILURE;
      }
    }
  }

  nus_binary_model_free(&read_model);
  return NUS_SUCCESS;
}
