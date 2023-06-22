#version 330 core


in vec3 position;
in vec3 acceleration;

out vec3 geom_acceleration;

void main()
{
    geom_acceleration = acceleration;
    gl_Position = vec4(position, 1.0);
}
