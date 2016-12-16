#include "vertex.h"
#include "vector.h"

vertex gen_vertex(void)
{
  vertex vert;
  vert.attribute[0] = 0;
  vert.attribute[1] = 0;
  vert.attribute[2] = 0;
  vert.attribute[3] = 0;
  vert.attribute[4] = 0;
  vert.attribute[5] = 0;
  vert.attribute[6] = 0;
  vert.attribute[7] = 0;
  return vert;
}
vertex build_vertex(vector pos, vector norm, vector tex)
{
  vertex vert;
  vert.attribute[VERTEX_POSITION_START + 0] = pos.x;
  vert.attribute[VERTEX_POSITION_START + 1] = pos.y;
  vert.attribute[VERTEX_POSITION_START + 2] = pos.z;
  vert.attribute[VERTEX_NORMAL_START + 0] = norm.x;
  vert.attribute[VERTEX_NORMAL_START + 1] = norm.y;
  vert.attribute[VERTEX_NORMAL_START + 2] = norm.z;
  vert.attribute[VERTEX_UV_START + 0] = tex.x;
  vert.attribute[VERTEX_UV_START + 1] = tex.y;
  return vert;
}
void free_vertex(vertex *vert)
{
  vert->attribute[0] = 0;
  vert->attribute[1] = 0;
  vert->attribute[2] = 0;
  vert->attribute[3] = 0;
  vert->attribute[4] = 0;
  vert->attribute[5] = 0;
  vert->attribute[6] = 0;
  vert->attribute[7] = 0;
}
vector get_vertex_position(vertex *ver)
{
  return build_vector(ver->attribute[VERTEX_POSITION_START + 0],
		      ver->attribute[VERTEX_POSITION_START + 1],
		      ver->attribute[VERTEX_POSITION_START + 2]);
}
vector get_vertex_normal(vertex *ver)
{
  return build_vector(ver->attribute[VERTEX_NORMAL_START + 0],
		      ver->attribute[VERTEX_NORMAL_START + 1],
		      ver->attribute[VERTEX_NORMAL_START + 2]);
}
vector get_vertex_tex_coords(vertex *ver)
{
  return build_vector(ver->attribute[VERTEX_UV_START + 0],
		      ver->attribute[VERTEX_UV_START + 1],
		      0.0);
}
