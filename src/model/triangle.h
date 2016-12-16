#ifndef TRIANGLE
#define TRIANGLE

struct vertex;
struct vector;

#define WINDING_UNDEFINED 0b0
#define WINDING_COUNTERCLOCKWISE 0b1
#define WINDING_CLOCKWISE 0b10

typedef struct triangle{
	unsigned int index[3];
	struct vertex *vertex_list;
} triangle;

triangle gen_triangle(void);
void free_triangle(triangle *);

void set_triangle_indices(triangle *, unsigned int, unsigned int, unsigned int);
void set_triangle_vertex_list(triangle *, struct vertex *);
struct vertex *get_triangle_vertex(triangle *, char);
char test_triangle_winding(triangle, struct vector);

#endif /* TRIANGLE */
