#version 450

layout(location = 0) in vec4 vert_color;

layout(location = 0) out vec4 frag_color;

void main() {
    frag_color = vert_color;

    // hard-code color for testing
    //frag_color = vec4(1.0, 0.6, 0.0, 1.0);
}