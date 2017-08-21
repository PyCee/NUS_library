#include "NUS_load.h"

#include <stdio.h>
#include <stdlib.h>

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // Include library for loading images

struct aiNode *nusm_find_node(struct aiNode *, char *);
NUS_result nusm_load_animation(NUS_binary_model_animation *, struct aiAnimation *);

NUS_result nusm_load(char * file_path, NUS_binary_model *p_binary_model)
{
  const struct aiScene* scene;
  struct aiMesh *mesh;
  NUS_binary_model_vertex *p_vertex;
  unsigned int i,
    j,
    k;
  scene = aiImportFile(file_path,
		       aiProcess_CalcTangentSpace       |
		       aiProcess_FlipWindingOrder       |
		       aiProcess_Triangulate            | 
		       aiProcess_JoinIdenticalVertices  | 
		       aiProcess_GenNormals	        | 
		       aiProcess_FlipUVs 	        |
		       aiProcess_GenUVCoords	        | 
		       aiProcess_SortByPType);
  if(scene == NULL){
    printf("NUSM_ERROR::failed to import file \"%s\"\n", file_path);
    return NUS_FAILURE;
  }
  mesh = scene->mMeshes[0];
  
  if(!mesh->mTextureCoords[0]){
    printf("NUSM_ERROR::model does not support texturing\n");
    return NUS_FAILURE;
  }
  
  int vertex_count = mesh->mNumVertices;
  int index_count = mesh->mNumFaces * 3;

  printf("Vertices\n");

  p_binary_model->vertex_data_size = vertex_count * NUS_BINARY_MODEL_VERTEX_SIZE;
  p_binary_model->vertices = malloc(p_binary_model->vertex_data_size);
  
  for(i = 0; i < vertex_count; ++i){
    // Strores vertex position data
    p_binary_model->vertices[i].position[0] = mesh->mVertices[i].x;
    p_binary_model->vertices[i].position[1] = mesh->mVertices[i].y;
    p_binary_model->vertices[i].position[2] = mesh->mVertices[i].z;

    // Stores vertex normal data
    p_binary_model->vertices[i].normal[0] = mesh->mNormals[i].x;
    p_binary_model->vertices[i].normal[1] = mesh->mNormals[i].y;
    p_binary_model->vertices[i].normal[2] = mesh->mNormals[i].z;

    // Stores vertex texture coord data
    p_binary_model->vertices[i].tex_coords[0] = mesh->mTextureCoords[0][i].x;
    p_binary_model->vertices[i].tex_coords[1] = mesh->mTextureCoords[0][i].y;

    p_binary_model->vertices[i].bone_weight[0] = 0.0;
    p_binary_model->vertices[i].bone_weight[1] = 0.0;
    p_binary_model->vertices[i].bone_weight[2] = 0.0;
    p_binary_model->vertices[i].bone_weight[3] = 0.0;
    
    p_binary_model->vertices[i].bone_indices[0] = 0;
    p_binary_model->vertices[i].bone_indices[1] = 0;
    p_binary_model->vertices[i].bone_indices[2] = 0;
    p_binary_model->vertices[i].bone_indices[3] = 0;
  }
  printf("\tCount: %d\n\tSize: %zu\n", vertex_count,
	 p_binary_model->vertex_data_size);
  
  printf("Indices\n");
  p_binary_model->index_data_size = index_count * NUS_BINARY_MODEL_INDEX_SIZE;
  p_binary_model->indices = malloc(p_binary_model->index_data_size);
  // Store index data
  for(i = 0; i < mesh->mNumFaces; ++i){
    p_binary_model->indices[(3 * i)] = mesh->mFaces[i].mIndices[0];
    p_binary_model->indices[(3 * i) + 1] = mesh->mFaces[i].mIndices[1];
    p_binary_model->indices[(3 * i) + 2] = mesh->mFaces[i].mIndices[2];
  }
  printf("\tCount: %d\n\tSize: %zu\n", index_count,
	 p_binary_model->index_data_size);
  
  printf("Texture\n");
  // Load texture file path
  struct aiString texture_path;
  aiGetMaterialString(scene->mMaterials[mesh->mMaterialIndex],
		      AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &texture_path);
  printf("\tFile Name: %s\n", texture_path.data);
  
  // Load texture
  int texture_channels,
    width,
    height;
  
  void *data = stbi_load(texture_path.data, &width, &height,
			 &texture_channels, STBI_rgb_alpha);
  if (data == NULL) {
    printf("NUSM_ERROR::failed to load texture data\n");
      return NUS_FAILURE;
  }
  
  p_binary_model->texture_width = width;
  p_binary_model->texture_height = height;
  p_binary_model->texture_data_size = width * height * 4;
  
  printf("\tWidth: %zu\n\tHeight: %zu\n\tSize: %zu\n", p_binary_model->texture_width,
	 p_binary_model->texture_height, p_binary_model->texture_data_size);
  // 1 byte for each component of rgba
  
  p_binary_model->texture_data = malloc(p_binary_model->texture_data_size);
  memcpy(p_binary_model->texture_data, data, p_binary_model->texture_data_size);
  free(data);

  printf("Skeleton\n");
  p_binary_model->joint_count = 0;
  p_binary_model->skeleton_data_size = 0;
  p_binary_model->animation_count = 0;
  if(mesh->mNumBones){
    printf("\tBone Count: %d\n", mesh->mNumBones);
    p_binary_model->joint_count = mesh->mNumBones;
    p_binary_model->skeleton_data_size =
      sizeof(*p_binary_model->skeleton_joints) * mesh->mNumBones;
    p_binary_model->skeleton_data = malloc(p_binary_model->skeleton_data_size);
    
    for(i = 0; i < mesh->mNumBones; ++i){
      // For each bone
      printf("\tBone %d\n", i);
      printf("\t\tName: %s\n", mesh->mBones[i]->mName.data);
      printf("\t\tCount of affected vertices: %d\n", mesh->mBones[i]->mNumWeights);

      // Record skeleton data
      memcpy(&p_binary_model->skeleton_joints[i].inv_bind_pose,
	     &mesh->mBones[i]->mOffsetMatrix, sizeof(mesh->mBones[i]->mOffsetMatrix));

      char parent_name[100];
      strcpy(parent_name,
	     nusm_find_node(scene->mRootNode,
			    mesh->mBones[i]->mName.data)->mParent->mName.data);
      printf("\t\tParent bone name: %s\n", parent_name);

      for(j = 0; j < mesh->mNumBones; ++j){
	if(strcmp(parent_name, mesh->mBones[j]->mName.data) == 0){
	  p_binary_model->skeleton_joints[i].parent_index = j;
	  break;
	}
      }
      memset(p_binary_model->skeleton_joints[i].name, 0,
	     sizeof(p_binary_model->skeleton_joints[i].name));
      strcpy(p_binary_model->skeleton_joints[i].name, mesh->mBones[i]->mName.data);
      
      int lowest_weight_i = 0;
      for(j = 0; j < mesh->mBones[i]->mNumWeights; ++j){
	// For each vertex affected by this bone
	p_vertex = p_binary_model->vertices + mesh->mBones[i]->mWeights[j].mVertexId;
	for(k = 1; k < 4; ++k){
	  // For each weight of the current vertex
	  if(p_vertex->bone_weight[k] < p_vertex->bone_weight[lowest_weight_i]){
	    // If k is the index of the lowest weight so far
	    lowest_weight_i = k;
	  }
	}
	if(mesh->mBones[i]->mWeights[j].mWeight >
	   p_vertex->bone_weight[lowest_weight_i]){
	  // If the bone's weight is greater than the vertexes lowest weight
	  p_vertex->bone_weight[lowest_weight_i] = mesh->mBones[i]->mWeights[j].mWeight;
	  p_vertex->bone_indices[lowest_weight_i] = i;
	}
      }
    }
    
    if(scene->mAnimations){
      printf("Animations\n");
      printf("\tCount: %d\n", scene->mNumAnimations);
      p_binary_model->animation_count = scene->mNumAnimations;
      p_binary_model->p_animations = malloc(sizeof(*p_binary_model->p_animations) *
					    scene->mNumAnimations);
      for(i = 0; i < scene->mNumAnimations; ++i){
	printf("\tAnimation %d\n", i);
	if(nusm_load_animation(p_binary_model->p_animations + i,
			       scene->mAnimations[i]) != NUS_SUCCESS){
	  printf("NUSM_ERROR::failed to load animation %d\n", i);
	  return NUS_FAILURE;
	}
	  //
      }
    } else{
      printf("\nNUSM_WARNING::mesh has no animations\n\n");
    }
  } else{
    printf("\nNUSM_WARNING::mesh has no skeleton\n\n");
  }
  
  return NUS_SUCCESS;
}

struct aiNode *nusm_find_node(struct aiNode *node, char *node_name)
{
  struct aiNode *result;
  int i;
  for(i = 0; i < node->mNumChildren; ++i){
    if(strcmp(node_name, node->mChildren[i]->mName.data) == 0){
      return node->mChildren[i];
    }
  }
  for(i = 0; i < node->mNumChildren; ++i){
    if((result = nusm_find_node(node->mChildren[i], node_name)) != NULL){
      return result;
    }
  }
  return NULL;
}
NUS_result nusm_load_animation
(NUS_binary_model_animation *p_binary_animation, struct aiAnimation *p_animation)
{
  float t;
  int frame_count,
    pre_index,
    post_index,
    i,
    j,
    k;
  struct aiNodeAnim *p_channel;
  
  char frame_exist_tracker[(int)p_animation->mDuration + 1];
  memset(frame_exist_tracker, 0, sizeof(frame_exist_tracker));
  
  printf("\t\tFull Name: %s\n", p_animation->mName.data);
  // Name skips the "Armature|" that is seen at the beginning of the full name
  printf("\t\tName: %s\n",
	 p_animation->mName.data + 9);
  printf("\t\tDuration: %f seconds\n",
	 p_animation->mDuration /
	 p_animation->mTicksPerSecond);
  printf("\t\tChannel Count: %d\n", p_animation->mNumChannels);
  frame_count = 0;
  for(i = 0; i < p_animation->mNumChannels; ++i){
    // For each bone
    p_channel = p_animation->mChannels[i];
    printf("\t\t\tChannel: %s\n", p_channel->mNodeName.data);
    if(p_channel->mNumPositionKeys != p_channel->mNumRotationKeys ||
       p_channel->mNumRotationKeys != p_channel->mNumScalingKeys){
      printf("NUSM_ERROR::position, rotation, and scaling key counts differ\n");
      return NUS_FAILURE;
    }
    for(j = 0; j < p_channel->mNumRotationKeys; ++j){
      // For each frame
      if(frame_exist_tracker[(int)p_channel->mRotationKeys[j].mTime] == 0){
	frame_exist_tracker[(int)p_channel->mRotationKeys[j].mTime] = 1;
	++frame_count;
      }
    }
  }
  
  printf("\t\tFrame Count: %d\n", frame_count);
  p_binary_animation->keyframe_count = frame_count;
  p_binary_animation->p_keyframe_joints =
    malloc(sizeof(*p_binary_animation->p_keyframe_joints) *
	   (p_animation->mNumChannels - 1) * frame_count);
  p_binary_animation->p_times =
    malloc(sizeof(*p_binary_animation->p_times) * frame_count);
  
  frame_count = 0;
  
  for(i = 0; i <= (int)p_animation->mDuration; ++i){
    // For each tick
    
    if(frame_exist_tracker[i] == 0){
      // If i is not the tick-time of a frame
      continue;
    }
    // Time in [0, 1] of animation duration
    p_binary_animation->p_times[frame_count] = (float)i / p_animation->mDuration;
    
    for(j = 0; j < p_animation->mNumChannels - 1; ++j){
      // For each bone
      p_channel = p_animation->mChannels[j + 1];
      pre_index = 0;
      post_index = 0;
      
      // Find the pre/post times for rot, trans, and scale
      for(k = 1; k < p_channel->mNumRotationKeys; ++k){
	if(p_channel->mRotationKeys[k].mTime > i){
	  post_index = k;
	  pre_index = post_index - 1;
	  break;
	}
      }
      
      t = (float)(i - p_channel->mRotationKeys[pre_index].mTime) /
	(p_channel->mRotationKeys[post_index].mTime -
	 p_channel->mRotationKeys[pre_index].mTime);
      // Lerp and store each transformation element
      p_binary_animation->p_keyframe_joints[frame_count *
					    (p_animation->mNumChannels - 1) + j] =
	(NUS_keyframe_joint){
	.rotation = {
	  .w = t * p_channel->mRotationKeys[pre_index].mValue.w + 
	  (1 - t) * p_channel->mRotationKeys[post_index].mValue.w,
	  .x = t * p_channel->mRotationKeys[pre_index].mValue.x + 
	  (1 - t) * p_channel->mRotationKeys[post_index].mValue.x,
	  .y = t * p_channel->mRotationKeys[pre_index].mValue.y + 
	  (1 - t) * p_channel->mRotationKeys[post_index].mValue.y,
	  .z = t * p_channel->mRotationKeys[pre_index].mValue.z + 
	  (1 - t) * p_channel->mRotationKeys[post_index].mValue.z
	},
	.translation = {
	  .x = t * p_channel->mPositionKeys[pre_index].mValue.x + 
	  (1 - t) * p_channel->mPositionKeys[post_index].mValue.x,
	  .y = t * p_channel->mPositionKeys[pre_index].mValue.y + 
	  (1 - t) * p_channel->mPositionKeys[post_index].mValue.y,
	  .z = t * p_channel->mPositionKeys[pre_index].mValue.z + 
	  (1 - t) * p_channel->mPositionKeys[post_index].mValue.z
	},
	.scale = {
	  .x = t * p_channel->mScalingKeys[pre_index].mValue.x + 
	  (1 - t) * p_channel->mScalingKeys[post_index].mValue.x,
	  .y = t * p_channel->mScalingKeys[pre_index].mValue.y + 
	  (1 - t) * p_channel->mScalingKeys[post_index].mValue.y,
	  .z = t * p_channel->mScalingKeys[pre_index].mValue.z + 
	  (1 - t) * p_channel->mScalingKeys[post_index].mValue.z
	}
      };
    }
    ++frame_count;
  }
  return NUS_SUCCESS;
}
