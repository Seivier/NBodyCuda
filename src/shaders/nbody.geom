#version 330 core

layout ( points ) in;
layout ( triangle_strip, max_vertices = 4 ) out;

in float geomMass[];

out float fragMass;

uniform bool useQuads;

void main(void){

    // vec4 dir_x = vec4(normalize(geom_acceleration[0]), 0.0);
    // rotate 90 degrees
    // vec4 dir_y = vec4(-dir_x.y, dir_x.x, 0.0, 0.0);
    if (useQuads)
    {
        vec4 offset = vec4(-1.0, 1.0, 0.0, 0.0);
        vec4 vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec4(-1.0, -1.0, 0.0, 0.0);
        vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec4(1.0, 1.0, 0.0, 0.0);
        vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec4(1.0, -1.0, 0.0, 0.0);
        vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();
    }
    else
    {
        vec4 offset = vec4(-1.0, -1.0, 0.0, 0.0);
        vec4 vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec4(1.0, -1.0, 0.0, 0.0);
        vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec4(0.0, 1.0, 0.0, 0.0);
        vertexPos = offset*0.01 + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();
    }


    EndPrimitive();
}