#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include <GL/glew.h>

struct matrix;

typedef struct shader_program{
  GLuint m_program, *m_uniform_locs;
  unsigned char *m_name, **m_uniform_names, m_uniform_count;
} shader_program;

shader_program gen_shader_program(void);
void free_shader_program(shader_program *);
shader_program build_shader_program(const unsigned char *,
				    const unsigned char *, const unsigned char *);
GLuint compile_shader_file(const unsigned char *, const GLenum);
GLuint get_shader_program_uniform_loc(shader_program, const unsigned char *);
void locate_shader_program_uniforms(shader_program *, unsigned char **, unsigned char);

#endif /* _SHADER_PROGRAM_H_ */
