#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 acceleration;

out vec3 geomAcceleration;

void main()
{
    geomAcceleration = acceleration;
    gl_Position = vec4(position, 1.0);
}
