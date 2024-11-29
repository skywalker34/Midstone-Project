#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shader_image_load_store : enable

layout(location = 0) out vec4 fragColor;

layout(location = 1) in vec2 textureCoords;

layout(binding = 0) uniform sampler2D screen;
in vec4 color;



void main() {

    //simply set the color to the color calculated in the vert
    fragColor = color;
}
