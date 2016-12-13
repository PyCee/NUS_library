#include <GL/glew.h>
#include <vertex.h>
#include <triangle.h>
#include <texture.h>
#include <model.h>
#include <mesh.h>
#include <matrix.h>
#include <vector.h>
#include <cartesian_system.h>
#include <frustum.h>
#include <SDL2/SDL.h>
#include "SOIL/SOIL.h"
#include <math.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>

#define PROGRAM_NAME "unit test - display animated model"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 650
#define LIFETIME 1000 * 1

#define MODEL_PATH "human_tpose.dae"

#define VERT_PATH "shader.vert"
#define FRAG_PATH "shader.frag"

int main(int argc, char *argv[])
{
	// Initialize Window and Opengl Context.
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* game_window = SDL_CreateWindow(PROGRAM_NAME, 500, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GLContext game_context = SDL_GL_CreateContext(game_window);
	glewInit();
	
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_CLAMP);
	
	glClearColor(0.4, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Load shader program from the vertex and fragment files.
	unsigned int program = glCreateProgram(),
				vert_sha = glCreateShader(GL_VERTEX_SHADER),
				frag_sha = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int file_len;
	int success;
	char *file_source;
	FILE *file;
	
	printf("Loading Vertex Shader %s\n", VERT_PATH);
	if((file = fopen(VERT_PATH, "r")) == NULL){
		printf("ERROR::File Not Found: %s\n", VERT_PATH);
		return 0;
	}
	fseek(file,  0, SEEK_END);
	file_len = ftell(file);
	fseek(file,  0, SEEK_SET);
	file_source = malloc(file_len);
	fread(file_source, file_len, 1, file);
	fclose(file);
	file = 0;
	{
		const char * c_file_source = file_source;
		const char * const * pc_file_source = &c_file_source;
		glShaderSource(vert_sha, 1, pc_file_source, &file_len);
	}
	glCompileShader(vert_sha);
	glGetShaderiv(vert_sha, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(vert_sha, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "ERROR::Compiling Shader Type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
	}
	free(file_source);
	file_source = 0;
	
	printf("Vertex Shader Loaded.\nLoading Fragment Shader %s\n", FRAG_PATH);
	if((file = fopen(FRAG_PATH, "r")) == NULL){
		printf("ERROR::File Not Found: %s\n", FRAG_PATH);
		return 0;
	}
	fseek(file,  0, SEEK_END);
	file_len = ftell(file);
	fseek(file,  0, SEEK_SET);
	file_source = malloc(file_len);
	fread(file_source, file_len, 1, file);
	fclose(file);
	file = 0;
	{
		const char * c_file_source = file_source;
		const char * const * pc_file_source = &c_file_source;
		glShaderSource(frag_sha, 1, pc_file_source, &file_len);
	}
	glCompileShader(frag_sha);
	glGetShaderiv(frag_sha, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(frag_sha, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "ERROR::Compiling Shader Type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog);
	}
	free(file_source);
	file_source = 0;
	
	printf("Fragment Shader Loaded.\n");
	glAttachShader(program, vert_sha);
	glAttachShader(program, frag_sha);
	
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindAttribLocation(program, 2, "uv_coords");
	
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success){
		GLchar InfoLog[1024];
		glGetProgramInfoLog(program, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "ERROR::Linking Shader Program: '%s'\n", InfoLog);
	}
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if(!success){
		GLchar InfoLog[1024];
		glGetProgramInfoLog(program, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "ERROR::Linking Shader Program: '%s'\n", InfoLog);
	}
	glUseProgram(program);
	
	// Find uniform variable locations.
	unsigned int wc_space_loc = glGetUniformLocation(program, "wc_space"),
				texture_map_loc = glGetUniformLocation(program, "texture_map");
	if(wc_space_loc == -1) printf("ERROR::wc_space_loc Not Found In Shader \"program\"\n");
	if(texture_map_loc == -1) printf("ERROR::texture_map_loc Not Found In Shader \"program\"\n");
	else glUniform1i(texture_map_loc, 0);
	
	// Set uniform variable values.
	mat4 world_trans = build_transformation_mat4(gen_cartesian_system(), build_vector(0.0, -1.0, -1.0));
	
	
	float aspect_ratio = 100.0/65.0;
	float fov = 90;
	float near = 0.101;
	float far = 100;
	float top = tan(fov / 2) * near;
	float bot = -1.0 * top;
	float rig = top * aspect_ratio;
	float lef = -1.0 * rig;
	frustum clip_frustum = build_frustum(near, far, top, bot, rig, lef);
	
	// Combining 2 transformation matrices into 1 transformation matrix before the per-vertex stage.
	mat4 wc_space_transformation = mult_mat4(*clip_frustum.transformation, world_trans);
	glUniformMatrix4fv(wc_space_loc, 1, GL_TRUE, &wc_space_transformation.mat[0][0]);
	// Load 3D model from file.
	model mod = build_model_from_file(MODEL_PATH);
	
	// Keep track of time to keep window open for set ammmount of time.
	unsigned long elapsed_time = 0, prev_frame_ms = SDL_GetTicks(), frame_ms = SDL_GetTicks();
	
	// Loop to keep window open observation.
	while(elapsed_time < LIFETIME){
		frame_ms = SDL_GetTicks();
		elapsed_time += frame_ms - prev_frame_ms;
		prev_frame_ms = frame_ms;
		
		SDL_GL_SwapWindow(game_window);
		SDL_Delay(1);
		glClearColor(0.4, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//TODO animate model with 3d library.
		//TODO work shader support for animations.
		
		render_model(mod);
	}
	
	// Clean up everything.
	SDL_Quit();
}
