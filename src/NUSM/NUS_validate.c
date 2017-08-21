#include "NUS_validate.h"
#include "NUS_store.h"
#include "NUS_read.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

NUS_result nusm_validate
(char *file_path, NUS_binary_model *p_binary_model)
{
  NUS_binary_model read_model;
  unsigned int i,
    j;

  /* Read model data from file */
  if(nusm_read(file_path, &read_model) == 0){
    printf("NUSM_ERROR::failed to read nusm file\n");
    return NUS_FAILURE;
  }
  
  /* Validate Vertex Data Size */
  if(read_model.vertex_data_size != p_binary_model->vertex_data_size){
    printf("NUSM_ERROR::failed to validate vertex data size \'%d\'\n",
	   (int)read_model.vertex_data_size);
    return NUS_FAILURE;
  }
  
  /* Validate Index Data Size */
  if(read_model.index_data_size != p_binary_model->index_data_size){
    printf("NUSM_ERROR::failed to validate index data size \'%d\'\n",
	   (int)read_model.index_data_size);
    return NUS_FAILURE;
  }
  
  /* Validate Texture Width */
  if(read_model.texture_width != p_binary_model->texture_width){
    printf("NUSM_ERROR::failed to validate texture width \'%d\'\n",
	   (int)read_model.texture_width);
    return NUS_FAILURE;
  }
  
  /* Validate Texture Height */
  if(read_model.texture_height != p_binary_model->texture_height){
    printf("NUSM_ERROR::failed to validate texture height \'%d\'\n",
	   (int)read_model.texture_height);
    return NUS_FAILURE;
  }
  
  /* Validate Texture Data Size */
  if(read_model.texture_data_size != p_binary_model->texture_data_size){
    printf("NUSM_ERROR::failed to validate texture data size \'%d\'\n",
	   (int)read_model.texture_data_size);
    return NUS_FAILURE;
  }
  
  /* Validate Vertex Data */
  for(i = 0; i < p_binary_model->vertex_data_size; ++i){
    if(((char*)read_model.vertex_data)[i] !=
       ((char*)p_binary_model->vertex_data)[i]){
      printf("NUSM_ERROR::failed to validate vertex data\n");
      return NUS_FAILURE;
    }
  }
  
  /* Validate Index Data */
  for(i = 0; i < p_binary_model->index_data_size / NUS_BINARY_MODEL_INDEX_SIZE; ++i){
    if(read_model.indices[i] != p_binary_model->indices[i]){
      printf("NUSM_ERROR::failed to validate index data\n");
      return NUS_FAILURE;
    }
  }
  
  /* Validate Texture Data*/
  for(i = 0; i < p_binary_model->texture_data_size; ++i){
    if(((char*)read_model.texture_data)[i] !=
       ((char*)p_binary_model->texture_data)[i]){
      printf("NUSM_ERROR::failed to validate texture data\n");
      return NUS_FAILURE;
    }
  }

  /* Validate Skeleton Data */
  for(i = 0; i < p_binary_model->skeleton_data_size; ++i){
    if(((char*)read_model.skeleton_data)[i] !=
       ((char*)p_binary_model->skeleton_data)[i]){
      printf("NUSM_ERROR::failed to validate skeleton data\n");
      return NUS_FAILURE;
    }
  }
  
  /* Validate Animation Data */
  for(i = 0; i < p_binary_model->animation_count; ++i){
    if(read_model.p_animations[i].keyframe_count !=
       p_binary_model->p_animations[i].keyframe_count){
      printf("NUSM_ERROR::failed to validate animation keyframe count\n");
      return NUS_FAILURE;
    }
    for(j = 0; j < sizeof(*p_binary_model->p_animations[0].p_keyframe_joints) *
	  p_binary_model->p_animations[i].keyframe_count; ++j){
      if(((char*)read_model.p_animations[i].p_keyframe_joints)[j] !=
	 ((char*)p_binary_model->p_animations[i].p_keyframe_joints)[j]){
	printf("NUSM_ERROR::failed to validate animation joint data\n");
	return NUS_FAILURE;
      }
    }
    for(j = 0; j < p_binary_model->p_animations[i].keyframe_count; ++j){
      if(read_model.p_animations[i].p_times[j] !=
	 p_binary_model->p_animations[i].p_times[j]){
	printf("NUSM_ERROR::failed to validate animation joint data\n");
	return NUS_FAILURE;
      }
    }
  }

  nus_binary_model_free(&read_model);
  return NUS_SUCCESS;
}
