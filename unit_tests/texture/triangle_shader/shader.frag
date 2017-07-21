#version 450

layout(location = 0) in vec2 frag_uv_coords;

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D tex_sampler;

void main() {
     frag_color = texture(tex_sampler, frag_uv_coords);

    // hard-code color for testing
    // frag_color = vec4(1.0, 0.6, 0.0, 1.0);
}