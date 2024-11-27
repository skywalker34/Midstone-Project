#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


layout(binding = 0) buffer layoutName
{
    vec3 data[];
} buf;


void main() {

    //this is either incredibly smart or incredibly stupid
    //I think this will solve the longstanding "vertex index" issue
    vec3 position = (modelMatrix * vVertex).xyz; //get the position of this vertex from the mesh.cpp vbo
    buf.data[gl_VertexID] = position; //send it onto my way cooler awesomer buffer
    gl_Position =vec4(1000,1000,1000,1);//send the vertext pos to timbucktoo so odds are you can't see it when it renders
    
}
