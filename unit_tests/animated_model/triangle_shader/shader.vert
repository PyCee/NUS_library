#version 450

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_color;
layout(location = 2) in vec2 vert_uv;
layout(location = 3) in vec4 vert_weights;
layout(location = 4) in uvec4 vert_weight_index;

layout(set = 0, binding = 0) uniform skinning_matrices_block{
  mat4 transformation[32];
} skinning_matrices;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout(location = 0) out vec4 frag_color; 
	 
void main()
{
  gl_Position = (skinning_matrices.transformation[vert_weight_index.x] *
  	      		vec4(vert_position, 1.0)) * vert_weights.x;
  gl_Position += (skinning_matrices.transformation[vert_weight_index.y] *
  	      		vec4(vert_position, 1.0)) * vert_weights.y;
  gl_Position += (skinning_matrices.transformation[vert_weight_index.z] *
  	      		vec4(vert_position, 1.0)) * vert_weights.z;
  gl_Position += (skinning_matrices.transformation[vert_weight_index.w] *
  	      		vec4(vert_position, 1.0)) * vert_weights.w;
  gl_Position = vec4(gl_Position.xyz / 10.0, 1.0);
  gl_Position.z += 0.7;
  //gl_Position = vec4(vert_position, 1.0);
  //frag_color = vec4(1.0, 1.0, 1.0, 0.0);
  //frag_color = vec4(skinning_matrices.transformation[0]);
  frag_color = vec4(vert_color.xyz, 1.0);
  //frag_color = vec4(gl_Position.z, gl_Position.z, gl_Position.z, 1.0);
}