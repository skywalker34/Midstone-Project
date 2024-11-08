#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColour;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textureCoords; 

uniform vec4 meshColor;


layout (binding = 0) uniform sampler2D diffMask; 


void main() {
    vec4 ks = vec4(0.3, 0.3, 0.3, 0.0);
	//vec4 kd = vec4(0.0, 0.0, 0.8, 0.0);
	//vec4 ka = vec4(0.001, 0.001, 0.001, 0.0);

	vec4 kd = vec4(1, 1, 1, 0.0) * meshColor;
	vec4 ka = vec4(0.1, 0.1, 0.1, 0.0);
	vec4 texColor = texture(diffMask,textureCoords); 
	vec4 kt = texColor;


    

	float diff = max(dot(vertNormal, lightDir), 0.0);

	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source so flip the sign
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));

	float spec = max(dot(eyeDir, reflection), 0.0);
	spec = pow(spec,1.0);
	vec4 outputColour = (ka + ((diff * kd) + (spec *ks)) * kt) ;	

	if(kt.w == 0){
		kt=vec4(0.2,0.2,0.2,0.6);
	}

	//mix with an atmosphere colour at the edges
	fragColour = kt;
	
	//fragColour = outputColour;
}