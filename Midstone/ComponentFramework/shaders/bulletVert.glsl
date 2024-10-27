#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPos;

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 lightDir;
layout(location = 2) out vec3 eyeDir;
layout(location = 3) out vec2 textureCoords; 
layout(location = 4) out vec3 incident;
layout(location = 5) out vec3 normal;


void main() {


    
    // We need the incident vector
    // First get the vertex position in world space
    vec3 vertexPosWorldSpace = (modelMatrix * vVertex).xyz; 

    
    incident = normalize(vertexPosWorldSpace - cameraPos);

    // We also need to rotate the normal to world space
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    normal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 

    
   
    vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
  
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
    
}
