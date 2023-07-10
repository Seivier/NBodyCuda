#version 330 core

in float fragMass;

out vec4 fragColor;

const float MAX_MASS = 1.0;

const float MIN_MASS = -0.5;

void main()
{
//    // interpolate between blue and red for high mass
//    vec4 hcolor = mix(vec4(0.0, 0.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), (fragMass - MID_MASS)/(MAX_MASS - MID_MASS));
//    // interpolate between green and hcolor for low mass
//    vec4 lcolor = mix(vec4(0.0, 1.0, 0.0, 1.0), hcolor, fragMass/MID_MASS);
    fragColor = mix(vec4(0.0, 1.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), (fragMass - MIN_MASS)/(MAX_MASS - MIN_MASS));
}
