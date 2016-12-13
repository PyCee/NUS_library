#ifndef _MESH_H_
#define _MESH_H_

struct aiMesh;
struct aiScene;

typedef struct mesh{
  struct vertex *vertices;
  struct texture *textures;
  struct triangle *triangles;
  unsigned int num_vertices,
    num_textures,
    num_triangles,
    VAO, VBO, EBO;
} mesh;

mesh gen_mesh(void);
void free_mesh(mesh *);
mesh build_mesh(struct aiMesh *, const struct aiScene *);

#endif /* _MESH_H_ */
