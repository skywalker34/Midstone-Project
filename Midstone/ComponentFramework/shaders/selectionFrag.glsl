#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec3 incident;
in vec3 normal;

uniform vec4 meshColor;
layout(location = 0) out vec4 fragColour;



void main(){
    
    //if its an edge, make it black, if its not, make it white to give everything a black bordder

    if(dot(normal, -incident) < 0.4){

       fragColour = meshColor;
    }
    else{

        fragColour =  vec4(0,0,0,0);
        
    }
}
