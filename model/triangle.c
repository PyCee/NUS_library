#include "triangle.h"
#include "vertex.h"
#include "vector.h"

triangle gen_triangle(void)
{
	triangle tri;
	tri.index[0] = 0;
	tri.index[1] = 0;
	tri.index[2] = 0;
	tri.vertex_list = 0;
	return tri;
}
void free_triangle(triangle *tri)
{
	tri->vertex_list = 0;
}
void set_triangle_indices(triangle *tri, unsigned int i0, unsigned int i1, unsigned int i2)
{
	tri->index[0] = i0;
	tri->index[1] = i1;
	tri->index[2] = i2;
}
void set_triangle_vertex_list(triangle *tri, vertex *list)
{
	tri->vertex_list = list;
}
struct vertex *get_triangle_vertex(triangle *tri, char index)
{
	if(tri->vertex_list)
		return &tri->vertex_list[tri->index[index]];
	return 0;
}
char test_triangle_winding(triangle tri, vector vec)
{
	vector a, b;
	vector vertex_position_1 = get_vertex_position(&tri.vertex_list[tri.index[1]]);
	a = sub_vector(vertex_position_1, get_vertex_position(&tri.vertex_list[tri.index[0]]));
	b = sub_vector(get_vertex_position(&tri.vertex_list[tri.index[2]]), vertex_position_1);
	vector cross = cross_vector(a, b);
	vector dir = sub_vector(vec, vertex_position_1);
	float dot = dot_vector(dir, cross);
	if(dot > 0) return WINDING_COUNTERCLOCKWISE;
	if(dot < 0) return WINDING_CLOCKWISE;
	return WINDING_UNDEFINED;
}
