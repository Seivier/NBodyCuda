#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in float mass;
layout (location = 2) in vec3 velocity;

out float fragMass;

const float size = 0.01;


void main()
{
    fragMass = length(velocity);
    gl_PointSize = size;
    gl_Position = vec4(position, 1.0);
}
