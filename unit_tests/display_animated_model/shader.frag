#version 130

in vec3 frag_normal;
in vec2 frag_uv_coords;

uniform sampler2D texture_map;

out vec4 clip_color;

void main()
{
	clip_color = texture(texture_map, frag_uv_coords);
	//clip_color = vec4(0.0, 0.4, 0.0, 1.0);
}
