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

