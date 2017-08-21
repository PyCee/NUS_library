#include "NUS_read.h"
#include "NUS_store.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

NUS_result nusm_read
(char *file_path, NUS_binary_model *p_binary_model)
{
  FILE *input_file;
  char magic_string[4];
  unsigned int i;

  printf("Reading file: \'%s\'\n", file_path);
  
  if((input_file = fopen(file_path, "rb")) == NULL){
    printf("NUSM_ERROR::failed to open \"%s\" for binary read\n", file_path);
    return NUS_FAILURE;
  }
  fseek(input_file, 0, SEEK_SET);

  /* Read Magic String */
  fread(magic_string, sizeof(char), 4, input_file);
  for(i = 0; i < 4; ++i){
    if(magic_string[i] != NUSM_MAGIC_STRING[i]){
      printf("NUSM_ERROR::invalid magic string\n\tValue %d is \'%d\'\n",
	     i, magic_string[i]);
      return NUS_FAILURE;
    }
  }

  if(!fread(&p_binary_model->vertex_data_size, sizeof(size_t) * 
	    NUS_BINARY_MODEL_SIZE_COUNT, 1, input_file)){
    printf("NUSM_ERROR::could not read size data\n");
    printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Vertex Data */
  p_binary_model->vertices = malloc(p_binary_model->vertex_data_size);
  if(!fread(p_binary_model->vertex_data, p_binary_model->vertex_data_size, 1,
	   input_file)){
    printf("NUSM_ERROR::could not read vertex data\n");
    printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Index Data */
  p_binary_model->index_data = malloc(p_binary_model->index_data_size);
  if(!fread(p_binary_model->index_data, p_binary_model->index_data_size, 1,
	   input_file)){
    printf("NUSM_ERROR::could not read index data\n");
    printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Texture Data */
  p_binary_model->texture_data = malloc(p_binary_model->texture_data_size);
  if(!fread(p_binary_model->texture_data, p_binary_model->texture_data_size, 1,
		    input_file)){
    printf("NUSM_ERROR::could not read texture data\n");
    printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }

  /* Read Skeleton Data */
  p_binary_model->skeleton_data = malloc(p_binary_model->skeleton_data_size);
  if(!fread(p_binary_model->skeleton_data, p_binary_model->skeleton_data_size, 1,
	   input_file)){
    printf("NUSM_ERROR::could not read skeleton data\n");
    printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
    return NUS_FAILURE;
  }
  
  /* Read Animation Data */
  
  p_binary_model->p_animations = malloc(p_binary_model->animation_count *
					sizeof(*p_binary_model->p_animations));
  for(i = 0; i < p_binary_model->animation_count; ++i){
    // For each animation

    // Read keyframe count
    if(!fread(&p_binary_model->p_animations[i].keyframe_count, sizeof(unsigned int),
	     1, input_file)){
      printf("NUSM_ERROR::could not read animation keyframe count\n");
      printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }

    // Read Keyframe Joint Data
    p_binary_model->p_animations[i].p_keyframe_joints =
      malloc(sizeof(*p_binary_model->p_animations[i].p_keyframe_joints) *
	     p_binary_model->joint_count *
	     p_binary_model->p_animations[i].keyframe_count);
    if(!fread(p_binary_model->p_animations[i].p_keyframe_joints,
	     sizeof(*p_binary_model->p_animations[i].p_keyframe_joints) *
	     p_binary_model->p_animations[i].keyframe_count *
	      p_binary_model->joint_count, 1, input_file)){
      printf("NUSM_ERROR::could not read animation keyframe joints\n");
      printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }

    // Read Keyframe Times
    p_binary_model->p_animations[i].p_times =
      malloc(sizeof(*p_binary_model->p_animations[i].p_times) *
	     p_binary_model->p_animations[i].keyframe_count);
    if(!fread(p_binary_model->p_animations[i].p_times,
	     sizeof(*p_binary_model->p_animations[i].p_times) *
	     p_binary_model->p_animations[i].keyframe_count,
	     1, input_file)){
      printf("NUSM_ERROR::could not read animation keyframe times\n");
      printf("ferror = %d\nfeof = %d\n", ferror(input_file), feof(input_file));
      return NUS_FAILURE;
    }
  }
    
  fclose(input_file);
  return NUS_SUCCESS;
}
