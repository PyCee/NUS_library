
#include "mesh.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SOIL/SOIL.h"
#include "triangle.h"
#include "vertex.h"
#include "texture.h"
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

mesh gen_mesh(void)
{
  mesh _mesh;
  _mesh.vertices = 0;
  _mesh.textures = 0;
  _mesh.triangles = 0;
  _mesh.num_vertices = 0;
  _mesh.num_textures = 0;
  _mesh.num_triangles = 0;
  _mesh.VAO = 0;
  _mesh.VBO = 0;
  _mesh.EBO = 0;
  return _mesh;
}
void free_mesh(mesh *_mesh)
{
  int i;
  if(_mesh->num_vertices){
    for(i = 0; i < _mesh->num_vertices; i++){
      free_vertex(_mesh->vertices + i);
    }
    free(_mesh->vertices);
    _mesh->vertices = 0;
    _mesh->num_vertices = 0;
  }
  if(_mesh->num_textures){
    for(i = 0; i < _mesh->num_textures; i++){
      free_texture(_mesh->textures + i);
    }
    free(_mesh->textures);
    _mesh->textures = 0;
    _mesh->num_textures = 0;
  }
  if(_mesh->num_triangles){
    for(i = 0; i < _mesh->num_triangles; i++){
      free_triangle(_mesh->triangles + i);
    }
    free(_mesh->triangles);
    _mesh->triangles = 0;
    _mesh->num_triangles = 0;
  }
}
mesh build_mesh(struct aiMesh *ai_mesh, const struct aiScene *scene)
{
  mesh _mesh;
  _mesh.num_vertices = ai_mesh->mNumVertices;
  _mesh.num_textures = 1;//ai_mesh->mNumTextures;
  _mesh.num_triangles = ai_mesh->mNumFaces;
  
  _mesh.vertices = malloc(sizeof(vertex) * _mesh.num_vertices);
  _mesh.textures = malloc(sizeof(texture) * _mesh.num_textures);
  _mesh.triangles = malloc(sizeof(triangle) * _mesh.num_triangles);
  
  unsigned int num_indices = ai_mesh->mNumFaces * 3;
  unsigned int *indices = malloc(sizeof(unsigned int) * num_indices);
  
  GLuint index;
  vertex vert;
  for(index = 0; index < ai_mesh->mNumVertices; index++){
    vert.attribute[0] = ai_mesh->mVertices[index].x;
    vert.attribute[1] = ai_mesh->mVertices[index].y;
    vert.attribute[2] = ai_mesh->mVertices[index].z;
    vert.attribute[3] = ai_mesh->mNormals[index].x;
    vert.attribute[4] = ai_mesh->mNormals[index].y;
    vert.attribute[5] = ai_mesh->mNormals[index].z;
    if(ai_mesh->mTextureCoords[0]){
      vert.attribute[6] = ai_mesh->mTextureCoords[0][index].x;
      vert.attribute[7] = ai_mesh->mTextureCoords[0][index].y;
    }
    _mesh.vertices[index] = vert;
  }
  for(index = 0; index < ai_mesh->mNumFaces; index++){
    _mesh.triangles[index] = gen_triangle();
    _mesh.triangles[index].vertex_list = _mesh.vertices;
    
    struct aiFace face = ai_mesh->mFaces[index];
    GLuint indice_index = 0;
    for(indice_index = 0; indice_index < face.mNumIndices; indice_index++){
      indices[(3 * index) + indice_index] = face.mIndices[indice_index];
      _mesh.triangles[index].index[indice_index] = face.mIndices[indice_index];
    }	
  }


  
  texture *_texture = &(_mesh.textures[0]);
  struct aiString textureString;
  aiGetMaterialString(scene->mMaterials[ai_mesh->mMaterialIndex],
		      AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &textureString);
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  *_texture = build_texture(texture_id,
			    "diffuse", textureString.data);
  
  glBindTexture(GL_TEXTURE_2D, texture_id);
  int width, height;
  unsigned char* image =
    SOIL_load_image(_texture->path, &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
	       image);
  SOIL_free_image_data(image);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  
  glGenVertexArrays(1, &_mesh.VAO);
  glGenBuffers(1, &_mesh.VBO);
  glGenBuffers(1, &_mesh.EBO);
  
  glBindVertexArray(_mesh.VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, _mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, _mesh.num_vertices * sizeof(vertex),
	       _mesh.vertices, GL_STATIC_DRAW); 
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int),
	       indices, GL_STATIC_DRAW);
  free(indices);
  indices = 0;
  
  // Vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)(VERTEX_POSITION_START * sizeof(float)));
  // Vertex Normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)(VERTEX_NORMAL_START * sizeof(float)));
  // Vertex Texture Coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)(VERTEX_UV_START * sizeof(float)));
  
  glBindVertexArray(0);
  return _mesh;
}
