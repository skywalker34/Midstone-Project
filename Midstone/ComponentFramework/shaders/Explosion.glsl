#version 450

layout(local_size_x = 1, local_size_y = 32, local_size_z = 1) in;

uniform int yDispatch;
uniform float simSpeed;
uniform float randSeed;


layout(binding = 0) buffer layoutPos
{
    vec3 posData[];
} buf;

layout(binding = 1) buffer layoutVel
{
    vec3 vel[];
} buf2;


float rand(float seed) { 
    return fract(sin(seed) * 43758.5453123); 
}

// Function to generate a random float between min and max using a seed
float randomFloat(float min, float max, float seed)
{
    return min + rand(seed) * (max - min);
}

vec3 getRandomVector(float radius, float distance, float seed, vec3 direction)
{
    float spreadAngle = atan(radius / distance);

    float randomAngle = randomFloat(-spreadAngle, spreadAngle, seed);
    float randomAzimuth = randomFloat(0.0, 2.0 * 3.141592, seed + 1.0); // Ensure different seed

    // Calculate the random direction using spherical coordinates
    vec3 randomDirection;
    randomDirection.x = sin(randomAngle) * cos(randomAzimuth);
    randomDirection.y = sin(randomAngle) * sin(randomAzimuth);
    randomDirection.z = cos(randomAngle);

    // Align the random direction with the weapon direction
    mat3 rotationMatrix = mat3(
        vec3(direction.x, direction.y, direction.z),
        vec3(-direction.y, direction.x, 0.0),
        vec3(-direction.z, 0.0, direction.x)
    );

    randomDirection = normalize(rotationMatrix * randomDirection);
    return randomDirection;
}


// Function to check if a point is within a cone
bool isPointInCone(vec3 point, vec3 apex, vec3 direction, float angle)
{
    // Calculate the vector from the cone's apex to the point
    vec3 apexToPoint = normalize(point - apex);

    // Normalize the direction vector (cone's axis)
    vec3 normDirection = normalize(direction);

    // Calculate the cosine of the angle between the two vectors
    float cosAngle = dot(normDirection, apexToPoint);

    // Calculate the cosine of the cone's angle
    float cosConeAngle = cos(angle);

    // If cosAngle is greater than cosConeAngle, the point is within the cone
    return cosAngle > cosConeAngle;
}




void main()
{
    // Calculate the unique index for the current work item
    uint index = gl_GlobalInvocationID.x + yDispatch * gl_GlobalInvocationID.y;
    //// Generate a unique seed for each particle
    float seed = randSeed * (float(index) * rand(randSeed));

    // Calculate the time step for the simulation
    float deltaTime = 1.0f / simSpeed;

    // Get the current position of the particle from the buffer
    vec3 pos = buf.posData[index];
    
    if(length(pos) == 0){
        float x = randomFloat(-100, 100, seed);
        seed = randSeed * (float(index) * rand(seed));
        float y = randomFloat(-100, 100, seed);
        seed = randSeed * (float(index) * rand(seed));
        float z = randomFloat(-100, 100, seed);
        seed = randSeed * (float(index) * rand(seed));

        //generate a random direction for the particle
        vec3 randomDirection = normalize(vec3(x,y,z));
        //generate a random force magnitude for the particle
        float forceMag = randomFloat(0, 1, seed);
        //"split" the force into a vector
        vec3 force = randomDirection * forceMag;

        // Calculate the acceleration of the particle (assuming mass = 0.1)
        vec3 accel = force / 0.1;
        buf2.vel[index] += accel * deltaTime;
    }
    
    //pos += buf2.vel[index] * deltaTime + 0.5f * accel * deltaTime * deltaTime;
   pos += buf2.vel[index];

    // Write the updated position back to the buffer
    buf.posData[index] = pos;
}



//// Function to simulate rand() using a seed
//float rand(float seed) { 
//    return fract(sin(seed) * 43758.5453123); 
//}
//
//// Function to generate a random float between min and max using a seed
//float randomFloat(float min, float max, float seed)
//{
//    return min + rand(seed) * (max - min);
//}
//
//vec3 getRandomVector(float radius, float dis, float seed, vec3 direction){
//     float spreadAngle = atan(radius / dis);
//
//     float randomAngle = randomFloat(-spreadAngle, spreadAngle, seed);
//
//     vec3 randomDirection = vec3(
//     direction.x + randomFloat(-radius,radius,seed),
//     direction.y + randomFloat(-radius,radius,seed),
//     direction.z + randomFloat(-radius,radius,seed)
//     );
//
//     randomDirection = normalize(randomDirection);
//     return randomDirection;
//}
//
//void main()
//{
//    
//    // Calculate the unique index for the current work item
//    uint index = gl_GlobalInvocationID.x + yDispatch * gl_GlobalInvocationID.y;
//    float seed = randSeed * float(index);
//    float deltaTime = 1.0f / simSpeed;
//
//    vec3 pos = buf.posData[index];
//    vec3 direction = normalize(-forwardVector);
//
//    if(pos == vec3(0.0f,0.0f,0.0f)){
//        direction = getRandomVector(0.1, 1,seed,direction);
//    }
//    else{
//        direction = normalize(buf2.vel[index]);
//    }
//    float forceMag = 0.1 * rand(seed);
//
//    vec3 force = direction * forceMag;
//    vec3 accel = force / 0.1;
//
//    pos += buf2.vel[index] * deltaTime + 0.5f * accel * deltaTime * deltaTime;
//    if((length(buf2.vel[index]) ) > 1.8){
//       // Update the particle velocity
//       buf2.vel[index] += accel * deltaTime;
//    }
//
//    if (length(pos) > 2)
//    {
//        pos = vec3(0.0, 0.0, 0.0);
//    }
//
//
//    buf.posData[index] = pos;
//    
//}






//everything below here is scrrp/garbage for possible compute shader implementations







//
//
//
//
//// Generate a unique seed for each particle
//    float seed = randSeed * float(index);
//
//    // Calculate the time step for the simulation
//    float deltaTime = 1.0f / simSpeed;
//
//    // Get the current position of the particle from the buffer
//    vec3 pos = buf.posData[index];
//
//    // Normalize the forward direction vector
//    vec3 direction = normalize(-forwardVector);
//
//    // Generate a random point within a cone for particle spread
//    // Random angle within the cone
//    float angle = acos(1.0 - rand(seed + 1.0) * (1.0 - cos(radians(180.0)))); // 50-degree cone
//
//    // Random azimuth angle around the cone
//    float azimuth = rand(seed +2) * 2.0 * 3.14159; // Full circle (2 * PI)
//
//    // Calculate the direction vector within the cone
//    vec3 coneDirection = vec3(sin(angle) * cos(azimuth), sin(angle) * sin(azimuth), cos(angle));
//    coneDirection = normalize(coneDirection);
//
//    // Mix the forward direction with the cone direction to align the particle spread with the forward vector
//    vec3 adjustedDirection = normalize(mix(direction, coneDirection, 0.9));
//
//    // Generate a random force magnitude for each particle
//    float forceMag = 0.1 * rand(seed);
//
//    // Calculate the force vector applied to the particle
//    vec3 force = adjustedDirection * forceMag;
//
//    // Calculate the acceleration of the particle (assuming mass = 0.1)
//    vec3 accel = force / 0.1;
//  
//
//    // Update the particle position using its velocity and acceleration
//    pos += buf2.vel[index] * deltaTime + 0.5f * accel * deltaTime * deltaTime;
//
//
//    //cap the particle speed
//    if((length(buf2.vel[index]) > 1.8) == false){
//        // Update the particle velocity
//        buf2.vel[index] += accel * deltaTime;
//    }
//
//    
//
//    // Reset the particle position if it moves too far
//    if (length(pos) > 2)
//    {
//        pos = vec3(0.0, 0.0, 0.0);
//    }
//
//    // Write the updated position back to the buffer
//    buf.posData[index] = pos;
//




















//layout(rgba32f, binding = 0) uniform image2D out_tex;

//layout(binding = 0) buffer Vertices {
//    vec3 vertices[];
//};
//
//layout(binding = 1) buffer UVs {
//    vec2 uvCoords[];
//};

//    ivec2 pos = ivec2( gl_GlobalInvocationID.xy );
//    //pos = ivec2(0,0);
//    vec3 color = vec3(1,0,0);
//    if(gl_GlobalInvocationID.x == 0 && gl_GlobalInvocationID.y == 0  ){
//        //first pixel should be white
//        color.x = 1;
//        color.y = 1;
//        color.z = 1;
//        pos.x += 1;
//        pos.y += 1;
//    }
//    else{
//        color = vec3(1,0,0);//all other pixels should be red
//        pos = ivec2(0,0);
//    }
//
//    imageStore(out_tex, pos, vec4(color, 0.0));


//
//#version 450
//
//layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in; //32 work groups since I'm running nvidia
////gl_WorkGroupID == gl_GlobalInvocationID
////layout(binding = 0) uniform image2D screen;
//layout(rgba32f, binding = 0) uniform image2D out_tex;
//
//
//
//
//uniform float ten;
//
//
//void main()
//{
//////  
////
////
////// get position to read/write data from
////    ivec2 pos = ivec2( gl_GlobalInvocationID.xy );
////    // get value stored in the image
////    vec3 in_val = imageLoad( out_tex, pos ).rgb;
////    float deltaTime = 0.1f;
////    float g = 1.0f;
////    float mass = 1.0f;
////
////    vec3 accel = vec3(0.0f, (g / mass), 0.0f);
////    vec3 vel;
////    vel.y += in_val.g + accel.y * deltaTime;
////
////
////    
////
////
////
////    //vec3 vel = vec3(0,in_val.g + 0.1,0);
//    
//    // store new value in image
//    //imageStore(out_tex, pos, vec4(color, vel.y, vel.z, 1.0 ) );
//    imageStore(out_tex, pos, vec4(ten, vel.y, vel.z, 1.0 ) );
// 
//
//
////vec4 pixel = vec4(0.075, 0.133, 0.172, 1.0);
////    ivec2 pixel_coords = ivec2(gl_LocalInvocationID.xy);
////
////    ivec2 dims = imageSize(screen);
////    float x = -(float(pixel_coords.x * 2 - dims.x) / dims.x);
////    float y = -(float(pixel_coords.y * 2 - dims.y) / dims.y);
////
////    float fov = 45;
////    vec3 cam_o = vec3(0.0, 0.0, -tan(fov / 2.0));
////    vec3 ray_o = vec3(x,y,0.0);
////
////    vec3 ray_d = normalize(ray_o - cam_o);
////
////    vec3 sphere_c = vec3(0.0, 0.0, -5.0);
////    float sphere_r = 1.0f;
////
////    vec3 o_c = ray_o - sphere_c;
////    float b = dot(ray_d, o_c);
////    float c = dot(o_c, o_c) - sphere_r * sphere_r;
////    float intersectionState = b * b - c;
////    vec3 intersection = ray_o + ray_d * (-b + sqrt(b *b - c));
////
////    if (intersectionState >= 0.0)
////    {
////        pixel = vec4((normalize(intersection - sphere_c) + 1.0) / 2.0, 1.0);
////
////    }
////   
////   pixel = vec4(1,0,0,1);
////   imageStore(screen, pixel_coords, pixel);
//
//    
//};