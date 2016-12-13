#include "texture.h"
#include <stdlib.h>

texture gen_texture(void)
{
  texture _texture;
  _texture.id = 0;
  _texture.type = NULL;
  _texture.path = NULL;
  return _texture;
}
void free_texture(texture *_texture)
{
  _texture->id = 0;
  if(_texture->type){
    free(_texture->type);
    _texture->type = NULL;
  }
  if(_texture->path){
    free(_texture->path);
    _texture->path = NULL;
  }
}
texture build_texture
(GLuint id, const unsigned char *type, const unsigned char *path)
{
  texture _texture = gen_texture();
  _texture.id = id;
  _texture.type = malloc(strlen(type) + 1);
  strcpy(_texture.type, type);
  _texture.path = malloc(strlen(path) + 1);
  strcpy(_texture.path, path);
  return _texture;
}
