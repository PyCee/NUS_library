#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL_opengl.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

typedef struct texture{
  GLuint id;
  char *type,
    *path;
} texture;

texture gen_texture(void);
void free_texture(texture *);
texture build_texture(GLuint, const unsigned char *, const unsigned char *);

#endif /* _TEXTURE_H_ */
