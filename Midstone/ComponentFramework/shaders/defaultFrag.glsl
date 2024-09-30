#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 3) uniform vec4 meshColor;

layout(location = 0) out vec4 fragColor;


void main() {
    fragColor = meshColor;
}