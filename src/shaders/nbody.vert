#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in float mass;

out float geomMass;

void main()
{
    geomMass = mass;
    gl_Position = vec4(position, 1.0);
}
