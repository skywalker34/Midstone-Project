#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;
uniform vec3 cameraPos;

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 lightDir;
layout(location = 2) out vec3 eyeDir;
layout(location = 3) out vec2 textureCoords;
layout(location = 4) out vec3 incident;

void main() {
    // Convert light position from world space to model space
    vec3 lightPosModel = vec3(inverse(modelMatrix) * vec4(lightPos, 1.0));
    
    vec3 vertexPosWorldSpace = (modelMatrix * vVertex).xyz;
    incident = normalize(vertexPosWorldSpace - cameraPos);

    textureCoords = uvCoord;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); // Rotate the normal to the correct orientation

    vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;

    lightDir = normalize(lightPosModel - vVertex.xyz);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}
