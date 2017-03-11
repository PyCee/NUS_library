#ifndef _VERTEX_H_
#define _VERTEX_H_

#define VERTEX_POSITION_START 0
#define VERTEX_NORMAL_START 3
#define VERTEX_UV_START 6

struct vector;

typedef struct vertex{
  float attribute[3/*position*/ + 3/*normal*/ + 2/*texture coordinates*/];
} vertex;

vertex gen_vertex(void);
void free_vertex(vertex *);
vertex build_vertex(struct vector, struct vector, struct vector);

struct vector get_vertex_position(vertex *);
struct vector get_vertex_normal(vertex *);
struct vector get_vertex_tex_coords(vertex *);

#endif /* _VERTEX_H_ */
