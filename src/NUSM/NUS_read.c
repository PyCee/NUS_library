#include "NUS_read.h"
#include "NUS_store.h"
#include "../NUS_log.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

NUS_result nusm_read
(char *file_path, NUS_binary_model *p_binary_model)
{
  FILE *input_file;
  size_t i,
    j;
  char magic_string[4];
  
  if((input_file = fopen(file_path, "rb")) == NULL){
    NUS_LOG_ERROR("failed to open \"%s\" for binary read\n", file_path);
    return NUS_FAILURE;
  }
  fseek(input_file, 0, SEEK_SET);

  /* Read Magic String */
  fread(magic_string, sizeof(char), 4, input_file);
  for(i = 0; i < 4; ++i){
    if(magic_string[i] != NUSM_MAGIC_STRING[i]){
      NUS_LOG_ERROR("invalid magic string \'%s\'\n", magic_string);
      return NUS_FAILURE;
    }
  }

  if(!fread(&p_binary_model->vertex_count, sizeof(uint32_t) *
	    NUS_BINARY_MODEL_SIZE_COUNT, 1,
	    input_file)){
    NUS_LOG_ERROR("could not read size data\n");
    NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Vertex Data */
  uint32_t vertex_data_size = p_binary_model->vertex_count *
    sizeof(*p_binary_model->vertices);
  p_binary_model->vertices = malloc(vertex_data_size);
  if(!fread(p_binary_model->vertices, vertex_data_size, 1, input_file)){
    NUS_LOG_ERROR("could not read vertex data\n");
    NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Index Data */
  uint32_t index_data_size = p_binary_model->index_count *
    sizeof(*p_binary_model->indices);
  p_binary_model->indices = malloc(index_data_size);
  if(!fread(p_binary_model->indices, index_data_size, 1, input_file)){
    NUS_LOG_ERROR("could not read index data\n");
    NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Texture Data */
  if(p_binary_model->texture_data_size){
    p_binary_model->texture_data = malloc(p_binary_model->texture_data_size);
    if(!fread(p_binary_model->texture_data, p_binary_model->texture_data_size, 1,
	      input_file)){
      NUS_LOG_ERROR("could not read texture data\n");
      NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
  }

  /* Read Skeleton Data */
  if(!fread(&p_binary_model->skeleton.joint_count,
	    sizeof(p_binary_model->skeleton.joint_count), 1, input_file)){
    NUS_LOG_ERROR("could not read skeleton joint count\n");
    NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  if(p_binary_model->skeleton.joint_count){
    size_t joint_data_size = p_binary_model->skeleton.joint_count *
      sizeof(*p_binary_model->skeleton.joints);
    p_binary_model->skeleton.joints = malloc(joint_data_size);
    if(!fread(p_binary_model->skeleton.joints, joint_data_size, 1, input_file)){
      NUS_LOG_ERROR("could not read skeleton joints\n");
      NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
  }
  
  /* Read Animation Data */
  
  p_binary_model->animations = malloc(p_binary_model->animation_count *
					sizeof(*p_binary_model->animations));
  for(i = 0; i < p_binary_model->animation_count; ++i){
    // For each animation
    
    // Read name
    if(!fread(&p_binary_model->animations[i].name,
	      NUS_ANIMATION_NAME_SIZE, 1, input_file)){
      NUS_LOG_ERROR("could not read animation name\n");
      NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
    
    // Read duration
    if(!fread(&p_binary_model->animations[i].duration,
	      sizeof(p_binary_model->animations[i].duration), 1, input_file)){
      NUS_LOG_ERROR("could not read animation duration\n");
      NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
    
    // Read keyframe count
    if(!fread(&p_binary_model->animations[i].frame_count,
	      sizeof(p_binary_model->animations[i].frame_count), 1, input_file)){
      NUS_LOG_ERROR("could not read animation keyframe count\n");
      NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
    
    // Read Keyframe Times
    size_t keyframe_times_data_size = p_binary_model->animations[i].frame_count *
      sizeof(*p_binary_model->animations[i].times);
    p_binary_model->animations[i].times = malloc(keyframe_times_data_size);
    if(!fread(p_binary_model->animations[i].times,
	     keyframe_times_data_size, 1, input_file)){
      NUS_LOG_ERROR("could not read animation keyframe times\n");
      NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
    
    // Read Keyframe Joint Data
    size_t keyframe_data_size = sizeof(*p_binary_model->animations[i].keyframes) *
      p_binary_model->animations[i].frame_count;
    p_binary_model->animations[i].keyframes = malloc(keyframe_data_size);
    
    for(j = 0; j < p_binary_model->animations[i].frame_count; ++j){
      // For each frame
      size_t keyframe_joint_data_size = p_binary_model->skeleton.joint_count *
	sizeof(*p_binary_model->animations[i].keyframes[j].joints);
      p_binary_model->animations[i].keyframes[j].joints =
	malloc(keyframe_joint_data_size);
      
      if(!fread(p_binary_model->animations[i].keyframes[j].joints,
	        keyframe_joint_data_size, 1, input_file)){
	NUS_LOG_ERROR("could not read animation keyframe joints\n");
	NUS_LOG_ERROR("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
	return NUS_FAILURE;
      }
    }
  }
  
  fclose(input_file);
  return NUS_SUCCESS;
}
