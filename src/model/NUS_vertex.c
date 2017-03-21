#include "NUS_vertex.h"

NUS_vertex nus_vertex_build
(NUS_vector position, NUS_vector normal, NUS_vector tex_coords)
{
  return (NUS_vertex){
    .attributes = {
      (float)position.x,
      (float)position.y,
      (float)position.z,
      (float)normal.x,
      (float)normal.y,
      (float)normal.z,
      (float)tex_coords.x,
      (float)tex_coords.y
    }
  };
}
void nus_vertex_free(NUS_vertex *p_vertex)
{

}
NUS_vector nus_vertex_position(NUS_vertex vertex)
{
  return nus_vector_build(vertex.attributes[0],
			  vertex.attributes[1],
			  vertex.attributes[2]);
}
NUS_vector nus_vertex_normal(NUS_vertex vertex)
{
  return nus_vector_build(vertex.attributes[3],
			  vertex.attributes[4],
			  vertex.attributes[5]);
}
NUS_vector nus_vertex_uv(NUS_vertex vertex)
{
  return nus_vector_build(vertex.attributes[6],
			  vertex.attributes[7],
			  0.0);
}
