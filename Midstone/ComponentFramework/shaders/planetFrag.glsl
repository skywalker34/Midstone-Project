#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColour;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textureCoords; 
layout(location = 4) in vec3 incident;


layout (binding = 0) uniform sampler2D diffTexture; 
layout (binding = 1) uniform sampler2D specTexture;

void main() {
    vec4 ks = vec4(0.3, 0.3, 0.3, 0.0);
	//vec4 kd = vec4(0.0, 0.0, 0.8, 0.0);
	//vec4 ka = vec4(0.001, 0.001, 0.001, 0.0);

	vec4 kd = vec4(1, 1, 1, 0.0);
	vec4 ka = vec4(0.1, 0.1, 0.1, 0.0);
	vec4 kt = texture(diffTexture,textureCoords); 
	
//	kd = vec4(0,0,0,1);
//	ka = vec4(0,0,0,1);
//	kt = vec4(0,0,0,1);



	float diff = max(dot(vertNormal, lightDir), 0.0);

	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source so flip the sign
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));

	float spec = max(dot(eyeDir, reflection), 0.0);
	spec = pow(spec,1.0) * texture(specTexture,textureCoords).x;
	vec4 outputColour = (ka + ((diff * kd) + (spec *ks)) * kt) ;	

	

	//mix with an atmosphere colour at the edges
	fragColour = mix(vec4(0.1f, 0.15f, 0.18f, 0.8f), outputColour, dot(vertNormal, -incident));
	
	//fragColour = outputColour;
}