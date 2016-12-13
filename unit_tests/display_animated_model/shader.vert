#version 130

in vec3 position;
in vec3 normal;
in vec2 uv_coords;

uniform mat4 wc_space;

out vec3 frag_normal;
out vec2 frag_uv_coords;

void main()
{
	gl_Position = wc_space * vec4(position, 1.0);
	frag_normal = vec3(world_space * vec4(normal, 0.0));
	frag_uv_coords = uv_coords;
}
