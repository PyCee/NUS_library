#include "shader_program.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <matrix.h>

/* The following variables are used to determine whether a shader file has
   already been compiled. If it has, the already compiled variable will be given*/
unsigned char **compiled_vertex_shader_paths,
  **compiled_fragment_shader_paths;
GLuint *compiled_vertex_shaders,
  *compiled_fragment_shaders;
short compiled_vertex_shader_count,
  compiled_fragment_shader_count;

static GLuint get_shader_uniform_loc(shader_program, const unsigned char *);

shader_program gen_shader_program(void)
{
  shader_program shader_program_;
  shader_program_.m_program = 0;
  shader_program_.m_name = 0;
  shader_program_.m_uniform_locs = 0;
  shader_program_.m_uniform_names = 0;
  shader_program_.m_uniform_count = 0;
  return shader_program_;
}
void free_shader_program(shader_program *shader_program_)
{
  if(shader_program_->m_uniform_count){
    free(shader_program_->m_uniform_locs);
    shader_program_->m_uniform_locs = 0;
    unsigned char i;
    for(i = 0; i < shader_program_->m_uniform_count; i++){
      free(shader_program_->m_uniform_names[i]);
    }
    free(shader_program_->m_uniform_names);
    shader_program_->m_uniform_names = 0;
    shader_program_->m_uniform_count = 0;
  }
  if(shader_program_->m_name){
    free(shader_program_->m_name);
    shader_program_->m_name = 0;
  }
  // free program shader object
}
shader_program build_shader_program(const unsigned char *name,
const unsigned char *vert_path, const unsigned char *frag_path)
{
  shader_program shader_program_;
  shader_program_.m_name = malloc(strlen(name) + 1);
  strcpy(shader_program_.m_name, name);

  GLuint  vert_shader, frag_shader;
  vert_shader = compile_shader_file(vert_path, GL_VERTEX_SHADER);
  frag_shader = compile_shader_file(frag_path, GL_FRAGMENT_SHADER);

  shader_program_.m_program = glCreateProgram();
  glAttachShader(shader_program_.m_program, vert_shader);
  glAttachShader(shader_program_.m_program, frag_shader);
  
  return shader_program_;
}
GLuint compile_shader_file(const unsigned char *shader_path, const GLenum shader_type)
{
  GLuint shader;
  
  unsigned char ***compiled_paths;
  GLuint **compiled_shaders;
  short *compiled_count;
  if(shader_type == GL_VERTEX_SHADER){
    compiled_paths = &compiled_vertex_shader_paths;
    compiled_shaders = &compiled_vertex_shaders;
    compiled_count = &compiled_vertex_shader_count;
  } else if(shader_type == GL_FRAGMENT_SHADER){
    compiled_paths = &compiled_fragment_shader_paths;
    compiled_shaders = &compiled_fragment_shaders;
    compiled_count = &compiled_fragment_shader_count;
  }
  short i;
  for(i = 0; i < *compiled_count; i++){
    if(strcmp((*compiled_paths)[i], shader_path) == 0){
      shader = (*compiled_shaders)[i];
      break;
    }
  }
  // if looped throught all pre-compiled shaders and the desired one was not found
  if(i == *compiled_count){
    (*compiled_count)++;
    if(i == 1){
      *compiled_paths = malloc(sizeof(**compiled_paths));
      *compiled_shaders = malloc(sizeof(**compiled_shaders));
    } else{
      *compiled_paths = realloc(*compiled_paths,
				sizeof(**compiled_paths) * i);
      *compiled_shaders = realloc(*compiled_shaders,
				  sizeof(**compiled_shaders) * i);
    }
    
    shader = glCreateShader(shader_type);
    unsigned int file_len;
    int success;
    char *file_source;
    FILE *file;
    
    if((file = fopen(shader_path, "r")) == NULL){
      printf("ERROR::File Not Found: %s\n", shader_path);
      return 0;
    }
    fseek(file,  0, SEEK_END);
    file_len = ftell(file);
    fseek(file,  0, SEEK_SET);
    file_source = malloc(file_len);
    fread(file_source, file_len, 1, file);
    fclose(file);
    file = 0;
    const char * c_file_source = file_source;
    const char * const * cc_file_source = &c_file_source;
    glShaderSource(shader, 1, cc_file_source, &file_len);
    
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLchar InfoLog[1024];
      glGetShaderInfoLog(shader, sizeof(InfoLog), NULL, InfoLog);
      fprintf(stderr, "ERROR::compiling shader type %d: '%s'\n", shader_type, InfoLog);
    }
    free(file_source);
    file_source = 0;
    
    
    (*compiled_paths)[i] = malloc(strlen(shader_path) + 1);
    strcpy((*compiled_paths)[i], shader_path);
    (*compiled_shaders)[i] = shader;
  }
  return shader;
}
GLuint get_shader_program_uniform_loc
(shader_program shader_program_, const unsigned char *uniform_name)
{
  unsigned char i;
  for(i = 0; i < shader_program_.m_uniform_count; i++){
    if(strcmp(shader_program_.m_uniform_names[i], uniform_name) == 0)
      return shader_program_.m_uniform_locs[i];
  }
  printf("ERROR::searching for uniform \'%s\' in shader %s\n",
	 uniform_name, shader_program_.m_name);
}
void locate_shader_program_uniforms
(shader_program *shader_program_, unsigned char **uniform_names,
 unsigned char uniform_count)
{
  shader_program_->m_uniform_count = uniform_count;
  shader_program_->m_uniform_locs = malloc(sizeof(*shader_program_->m_uniform_locs) *
					   uniform_count);
  shader_program_->m_uniform_names = malloc(sizeof(*shader_program_->m_uniform_names) *
					    uniform_count);
  unsigned char i;
  for(i = 0; i < uniform_count; i++){
    shader_program_->m_uniform_names[i] = malloc(strlen(uniform_names[i]) + 1);
    strcpy(shader_program_->m_uniform_names[i], uniform_names[i]);
    shader_program_->m_uniform_locs[i] =
      get_shader_uniform_loc(*shader_program_, uniform_names[i]);
  }
  // link and validate shader?
}
static GLuint get_shader_uniform_loc
(shader_program shader_program_, const unsigned char *uniform_name)
{
  GLuint uniform_loc = glGetUniformLocation(shader_program_.m_program, uniform_name);
  if(uniform_loc == -1) printf("ERROR::\'%s\' uniform not found in program %s\n",
			       uniform_name, shader_program_.m_name);
}
