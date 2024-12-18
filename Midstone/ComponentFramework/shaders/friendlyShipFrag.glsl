#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColour;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textureCoords; 

uniform vec4 primaryColour;
uniform vec4 secondaryColour;
uniform vec4 tertiaryColour;

layout (binding = 0) uniform sampler2D diffMask; 


void main() {
    vec4 ks = vec4(0.3, 0.3, 0.3, 0.0);


	vec4 kd = vec4(1, 1, 1, 0.0);
	vec4 ka = vec4(0.1, 0.1, 0.1, 0.0);
	vec4 texColor = texture(diffMask,textureCoords); 
	vec4 kt;





    vec4 secondary = secondaryColour; //colour to replace the scondary on the colour mask
    vec4 primary = primaryColour; //colour to replace the primary on the mask
    vec4 tertiary = tertiaryColour; //colour to replace the tertiary on the mask

	//mix the user-colours with the mask colours
    vec4 greenReplacement = mix(texColor, secondary, texColor.g);
    vec4 redReplacement = mix(texColor, primary, texColor.r);
    vec4 blueReplacement = mix(texColor, tertiary, texColor.b);

    // get teh combined texture colour
    kt = vec4(
        blueReplacement.r * texColor.b + greenReplacement.r * texColor.g + redReplacement.r * texColor.r,
        blueReplacement.g * texColor.b + greenReplacement.g * texColor.g + redReplacement.g * texColor.r,
        blueReplacement.b * texColor.b + greenReplacement.b * texColor.g + redReplacement.b * texColor.r,
        1.0
		);
    
	//Phong follows:

	float diff = max(dot(vertNormal, lightDir), 0.0);

	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source so flip the sign
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));

	float spec = max(dot(eyeDir, reflection), 0.0);
	spec = pow(spec,1.0);
	vec4 outputColour = (ka + ((diff * kd) + (spec *ks)) * kt) ;	
	

	fragColour = outputColour = (ka + ((diff * kd) + (spec *ks)) * kt);

}