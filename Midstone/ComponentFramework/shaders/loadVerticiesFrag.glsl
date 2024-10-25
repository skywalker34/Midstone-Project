#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shader_image_load_store : enable

layout(location = 0) out vec4 fragColor;





void main() {
    //make the color blakck cause there is nothing to see here
    fragColor = vec4(0,0,0,1);
}
