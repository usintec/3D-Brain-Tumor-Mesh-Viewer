#version 430 core

layout(location=0)in vec2 aPos;
layout(location=1)in vec2 aUV;

//------------------------------------
// Particle structure
//------------------------------------

struct Particle
{
    vec2 position;
    vec2 velocity;
    
    float density;
    float pressure;
    
    vec2 force;
};

//------------------------------------
// SSBO
//------------------------------------

layout(std430,binding=0)
buffer ParticleBuffer
{
    Particle particles[];
};

//------------------------------------
// Outputs
//------------------------------------

out vec2 UV;
out float DensityNorm;
out float SpeedVal;

//------------------------------------
// Main
//------------------------------------

void main()
{
    //--------------------------------
    // Instance = particle
    //--------------------------------
    
    uint id=
    gl_InstanceID;
    
    Particle p=
    particles[id];
    
    //--------------------------------
    // Particle size
    //--------------------------------
    
    float size=.055;
    
    //--------------------------------
    // Quad expansion
    //--------------------------------
    
    vec2 worldPos=
    p.position
    +aPos*size;
    
    //--------------------------------
    // Final position
    //--------------------------------
    
    gl_Position=
    vec4(
        worldPos,
        0.,
        1.
    );
    
    //--------------------------------
    // Outputs
    //--------------------------------
    
    UV=aUV;
    
    DensityNorm=
    clamp(
        p.density/20.,
        0.,
        1.5
    );
    
    SpeedVal=
    length(p.velocity);
}