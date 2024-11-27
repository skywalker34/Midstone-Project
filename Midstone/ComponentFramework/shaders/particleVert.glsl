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


    vec4 localPos =   vec4(buf.data[gl_VertexID], 1.0);

    vec4 nearColor = vec4(1,1,1,1);
    vec4 farColor = vec4(1.0f, 0.6f, 0.0f, 1.0f);

    color = mix(nearColor, farColor, length(localPos) / 2.4);

    vec3 position = (modelMatrix * localPos).xyz;


    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
    
}
