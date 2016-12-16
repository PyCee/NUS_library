#ifndef _MODEL_H_
#define _MODEL_H_

#define DEFAULT_MODEL {NULL, NULL, 0}

typedef struct model{
  struct mesh *meshes;
  char *path;
  unsigned int num_meshes;
} model;

model gen_model(void);
void free_model(model *);
model build_model(const char *);
void render_model(model);

#endif /* _MODEL_H_ */
