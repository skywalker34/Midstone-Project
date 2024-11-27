#version 450
#extension GL_ARB_separate_shader_objects : enable



layout(location = 4) in vec3 incident;
layout(location = 5) in vec3 normal;


layout(location = 0) out vec4 fragColour;



void main(){

    //psuedo glow, go!
	//we're pretty much just doing a temu fresnel effect with white and orange(bullet color customization?)
	fragColour =  mix(vec4(1, 0.6, 0.0f, 1.0f), vec4(1,1,1,1), dot(normal, -incident));
	
	
  
}

