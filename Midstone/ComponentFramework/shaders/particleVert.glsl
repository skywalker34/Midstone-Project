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

layout(location = 1) out vec2 textureCoords; 
layout(binding = 0) uniform sampler2D screen;

out vec4 color;



void main() {
    vec3 position = (modelMatrix * vVertex).xyz;

    //color = vec4(buf.data[gl_VertexID], 0.0);

    position += buf.data[gl_VertexID];

    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
    
}