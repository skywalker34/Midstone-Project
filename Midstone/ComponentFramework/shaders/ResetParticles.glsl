#version 450

layout(local_size_x = 1, local_size_y = 32, local_size_z = 1) in;

uniform int yDispatch;



layout(binding = 0) buffer layoutPos
{
    vec3 posData[];
} buf;

layout(binding = 1) buffer layoutVel
{
    vec3 vel[];
} buf2;




void main()
{
    uint index = gl_GlobalInvocationID.x + yDispatch * gl_GlobalInvocationID.y;
    //real simple compute shader that allows me to reset all the particles back to origin
    buf.posData[index] = vec3(0,0,0);
    buf2.vel[index] = vec3(0,0,0);
}

