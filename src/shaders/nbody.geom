#version 330 core

layout ( points ) in;
layout ( triangle_strip, max_vertices = 4 ) out;

in vec3 geom_acceleration[];

out vec3 frag_acceleration;

uniform float aspectRatio;

void main(void){

    // vec4 dir_x = vec4(normalize(geom_acceleration[0]), 0.0);
    // rotate 90 degrees
    // vec4 dir_y = vec4(-dir_x.y, dir_x.x, 0.0, 0.0);
    vec4 aspect = vec4(1.0, aspectRatio, 1.0, 1.0);

    vec4 offset = vec4(-1.0, 1.0, 0.0, 0.0);
    vec4 vertexPos = offset*0.1 + gl_in[0].gl_Position;
    gl_Position = vertexPos;
    frag_acceleration = geom_acceleration[0];
    EmitVertex();

    offset = vec4(-1.0, -1.0, 0.0, 0.0);
    vertexPos = offset*0.1 + gl_in[0].gl_Position;
    gl_Position = vertexPos;
    frag_acceleration = geom_acceleration[0];
    EmitVertex();

    offset = vec4(1.0, 1.0, 0.0, 0.0);
    vertexPos = offset*0.1 + gl_in[0].gl_Position;
    gl_Position = vertexPos;
    frag_acceleration = geom_acceleration[0];
    EmitVertex();

    offset = vec4(1.0, -1.0, 0.0, 0.0);
    vertexPos = offset*0.1 + gl_in[0].gl_Position;
    gl_Position = vertexPos;
    frag_acceleration = geom_acceleration[0];
    EmitVertex();

    EndPrimitive();
}