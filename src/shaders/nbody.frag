#version 330 core

in vec3 frag_acceleration;

out vec4 frag_color;

void main()
{

    frag_color = vec4(frag_acceleration, 1.0);
}
