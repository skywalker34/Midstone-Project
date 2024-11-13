#version 450 
#extension GL_ARB_separate_shader_objects : enable


out vec4 FragColor;
in flat vec3 Color;

void main()
{
    FragColor = vec4(0,1,0, 1.0);
}