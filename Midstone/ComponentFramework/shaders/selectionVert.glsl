#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec4 vNormal;


layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;
layout(location = 3) uniform vec3 cameraPos;


out vec3 incident; 
out vec3 normal;

void main() {

//stolen from physics class:
    // We need the incident vector
    // First get the vertex position in world space
    vec3 vertexPosWorldSpace = (modelMatrix * vVertex).xyz; // That last bit is called a swizzle

    // Umer fixed it by believing in cameras (sorry Scott)
    incident = normalize(vertexPosWorldSpace - cameraPos);

    // We also need to rotate the normal to world space
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    normal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}