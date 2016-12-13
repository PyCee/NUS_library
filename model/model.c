#include "model.h"
#include <stdlib.h>
#include "stdio.h"
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include <GL/glew.h>
#include "mesh.h"
#include "texture.h"
#include "vertex.h"

static int mesh_index;
static unsigned int count_meshes_from_aiScene(struct aiNode *);
static mesh *process_assimp_root_node
(unsigned int, struct aiNode *, const struct aiScene *);
static unsigned int process_assimp_node
(mesh *, struct aiNode *, const struct aiScene *);

model gen_model(void)
{
  static model _model = DEFAULT_MODEL;
  return _model;
}
void free_model(model *model_)
{
  unsigned char i = 0;
  if(model_->meshes){
    for(i = 0; i < model_->num_meshes; i++)
      free_mesh(model_->meshes + i);
    free(model_->meshes);
    model_->meshes = NULL;
  }
  model_->num_meshes = 0;
  if(model_->path){
    free(model_->path);
    model_->path = NULL;
  }
}
model build_model(const char *model_path)
{
  model model_ = gen_model();
  model_.path = malloc(strlen(model_path) + 1);
  strcpy(model_.path, model_path);
  // TODO: replace all of the assimp importing with own library
  //	2 parts:
  // 1. convert file into custom file type.
  // 2. import vertex info (positions, textures, normals, skeleton...)
  
  // later~
  // import animations
  const struct aiScene* scene = aiImportFile(model_path,
					     aiProcess_CalcTangentSpace       |
					     aiProcess_Triangulate            |
					     aiProcess_JoinIdenticalVertices  |
					     aiProcess_GenSmoothNormals	      |
					     aiProcess_GenUVCoords	      |
					     aiProcess_SortByPType);
  model_.num_meshes = count_meshes_from_aiScene(scene->mRootNode);
  model_.meshes = process_assimp_root_node(model_.num_meshes, scene->mRootNode, scene);
  return model_;
}
void render_model(model model_)
{
  unsigned int i = 0;
  for(i = 0; i < model_.num_meshes; i++){
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, model_.meshes[i].textures[0].id);
    
    glBindVertexArray(model_.meshes[i].VAO);
    glDrawElements(GL_TRIANGLES, model_.meshes[i].num_triangles * 3,
		   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
static unsigned int count_meshes_from_aiScene(struct aiNode *node)
{
  unsigned int num_meshes = node->mNumMeshes;
  unsigned int i = 0;
  for(i = 0; i < node->mNumChildren; i++){
    num_meshes += count_meshes_from_aiScene(node->mChildren[i]);
  }
  return num_meshes;
}
static mesh *process_assimp_root_node
(unsigned int num_meshes, struct aiNode *node, const struct aiScene *scene)
{
  mesh *mesh_ = malloc(sizeof(mesh) * num_meshes);
  unsigned int num_meshes_processed, i;
  for(i = 0; i < node->mNumMeshes; i++){
    struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
    mesh_[i] = build_mesh(ai_mesh, scene);
  }
  num_meshes_processed = node->mNumMeshes;
  for(i = 0; i < node->mNumChildren; i++){
    num_meshes_processed += process_assimp_node(mesh_ + num_meshes_processed,
						node->mChildren[i], scene);
  }
  return mesh_;
}
static unsigned int process_assimp_node
(mesh *mesh_, struct aiNode *node, const struct aiScene *scene)
{
  unsigned int num_meshes_processed, i;
  for(i = 0; i < node->mNumMeshes; i++){
    mesh_[i] = build_mesh(scene->mMeshes[node->mMeshes[i]], scene);
  }
  num_meshes_processed = node->mNumMeshes;
  for(i = 0; i < node->mNumChildren; i++){
    num_meshes_processed += process_assimp_node(mesh_ + num_meshes_processed,
						node->mChildren[i], scene);
  }
  return num_meshes_processed;
}
