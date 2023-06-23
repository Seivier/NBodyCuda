#version 330 core

in float fragMass;

out vec4 fragColor;

const float MAX_MASS = 100.0;

const float MID_MASS = 30.0;

void main()
{
    // interpolate between green and blue for middle mass
    vec4 hcolor = mix(vec4(0.0, 0.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), (fragMass - MID_MASS)/(MAX_MASS - MID_MASS));
    // interpolate between red and color for low mass
    vec4 lcolor = mix(vec4(0.0, 1.0, 0.0, 1.0), hcolor, fragMass/MID_MASS);
    fragColor = lcolor;
}
