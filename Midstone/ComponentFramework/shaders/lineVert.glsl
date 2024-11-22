#version 450 
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 aPos;




uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

}