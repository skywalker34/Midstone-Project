#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec3 uvwCoords;
uniform samplerCube skyboxTexture;
layout(location = 0) out vec4 fragColor;

void main() {
//get the texture color and set it
    fragColor = texture(skyboxTexture, uvwCoords);
}