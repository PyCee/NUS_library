#include "NUS_model.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "../NUS_log.h"
#include "NUS_binary_model.h"

#include "../NUSM/NUS_read.h"

NUS_result nus_model_build(NUS_absolute_path absolute_path, NUS_model *p_model)
{
  int i;
  NUS_binary_model binary_model;
  if(nusm_read(absolute_path.path, &binary_model) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to read model file \"%s\"\n", absolute_path.path);
    return NUS_FAILURE;
  }
  
  if(nus_memory_map_build(binary_model.vertex_count * sizeof(*binary_model.vertices),
			  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			  &p_model->vertex_memory) != NUS_SUCCESS){
    printf("ERROR::failed to build memory map for vertices\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(p_model->vertex_memory, binary_model.vertices,
			  binary_model.vertex_count * sizeof(*binary_model.vertices)) !=
     NUS_SUCCESS){
    printf("ERROR::failed to flush vertex memory\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_build(binary_model.index_count * sizeof(*binary_model.indices),
			  VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			  &p_model->index_memory) != NUS_SUCCESS){
    printf("ERROR::failed to build memory map for indices\n");
    return NUS_FAILURE;
  }
  if(nus_memory_map_flush(p_model->index_memory, binary_model.indices,
			  binary_model.index_count * sizeof(*binary_model.indices)) !=
     NUS_SUCCESS){
    printf("ERROR::failed to flush index memory\n");
    return NUS_FAILURE;
  }
  if(binary_model.texture_data_size != 0){
    if(nus_texture_build(binary_model.texture_width, binary_model.texture_height,
			 VK_FORMAT_R8G8B8A8_UNORM,
			 VK_IMAGE_USAGE_TRANSFER_DST_BIT |
			 VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
			 VK_IMAGE_USAGE_SAMPLED_BIT,
			 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			 VK_IMAGE_ASPECT_COLOR_BIT,
			 &p_model->texture) != NUS_SUCCESS){
      NUS_LOG_ERROR("failed to build model texture\n");
      return NUS_FAILURE;
    }
    if(nus_texture_buffer_image(binary_model.texture_data,
				binary_model.texture_data_size,
				&p_model->texture) != NUS_SUCCESS){
      NUS_LOG_ERROR("failed to load model texture data\n");
      return NUS_FAILURE;
    }
    p_model->has_texture = NUS_TRUE;
  } else {
    p_model->has_texture = NUS_FALSE;
  }

  if(binary_model.skeleton.joint_count != 0){
    p_model->skeleton = nus_skeleton_build(&binary_model.skeleton);
    
    if(binary_model.animation_count != 0){
      p_model->animation_count = binary_model.animation_count;
      p_model->animations = malloc(sizeof(*p_model->animations) *
				   binary_model.animation_count);
      for(i = 0; i < binary_model.animation_count; ++i){
	p_model->animations[i] =
	  nus_animation_build(&p_model->skeleton, binary_model.animations + i);
      }
    }
    p_model->has_skeleton = NUS_TRUE;
  } else{
    p_model->has_skeleton = NUS_FALSE;
  }
  nus_binary_model_free(&binary_model);
  p_model->binding = nus_get_binding();
  
  return NUS_SUCCESS;
}
void nus_model_free(NUS_model *p_model)
{
  nus_bind_binding(&p_model->binding);
  nus_memory_map_free(&p_model->vertex_memory);
  nus_memory_map_free(&p_model->index_memory);
  if(p_model->has_texture == NUS_TRUE){
    nus_texture_free(&p_model->texture);
  }
  if(p_model->has_skeleton == NUS_TRUE){
    nus_skeleton_free(&p_model->skeleton);
  }
  if(p_model->animation_count){
    for(int i = 0; i < p_model->animation_count; ++i){
      nus_animation_free(p_model->animations + i);
    }
    free(p_model->animations);
  }
  nus_unbind_binding(&p_model->binding);
}
