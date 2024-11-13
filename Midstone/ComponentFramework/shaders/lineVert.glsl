#version 450 
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out flat vec3 Color;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    Color = aColor;
}