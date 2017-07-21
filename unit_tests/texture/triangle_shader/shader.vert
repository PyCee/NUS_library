#version 450

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_color;
layout(location = 2) in vec2 vert_uv_coords;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout(location = 0) out vec2 frag_uv_coords; 
	 
void main()
{
  gl_Position = vec4(vert_position.xyz, 1.0);
  frag_uv_coords = vert_uv_coords;
}