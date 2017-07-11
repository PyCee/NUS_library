#version 450

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_color;

layout(set = 0, binding = 0) uniform transformation_block{
  mat4 world;
} transformation;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout(location = 0) out vec4 frag_color; 
	 
void main()
{
  gl_Position = transformation.world * vec4(vert_position.xyz, 1.0);
  frag_color = vec4(vert_color.xyz, 1.0);
}