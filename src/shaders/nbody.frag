#version 330 core

in vec3 fragAcceleration;

out vec4 fragColor;

const float MAX_ACCELERATION = 1.5;

void main()
{
    // choose a color base on acceleration
    float accel = length(fragAcceleration);
    // linear interpolation between red and green
    fragColor = mix(vec4(0.0, 1.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), accel/ MAX_ACCELERATION);
}
