#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 velocity;

out vec3 geomVelocity;

void main()
{
    geomVelocity = velocity;
    gl_Position = vec4(position, 1.0);
}
