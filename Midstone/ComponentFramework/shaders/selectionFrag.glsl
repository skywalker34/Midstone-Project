#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec3 incident;
in vec3 normal;

uniform vec4 meshColor;
layout(location = 0) out vec4 fragColour;



void main(){
    
    //if its an edge, make it the specified colour, if its not, make it seethrough
    //this gives the mesh the effect of being a 2d UI circle around an object

    if(dot(normal, -incident) < 0.4){

       fragColour = meshColor;
    }
    else{

        fragColour =  vec4(0,0,0,0);
        
    }
}
