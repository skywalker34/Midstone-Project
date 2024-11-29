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

    //pull the position from the buf that was written from teh compute shader (all local model spaces)
    vec4 localPos =   vec4(buf.data[gl_VertexID], 1.0);

    
    vec4 nearColor = vec4(1,1,1,1);//color to be used for particles near the origin (white)
    vec4 farColor = vec4(1.0f, 0.6f, 0.0f, 1.0f);//color to be used for paritcles far from the origin (orange)

    color = mix(nearColor, farColor, length(localPos) / 2.4); //lerp to get what color the particle should be

    vec3 position = (modelMatrix * localPos).xyz; //get teh position

    //pass it off to the frag
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
    
}
