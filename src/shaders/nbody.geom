#version 330 core

layout ( points ) in;
layout ( triangle_strip, max_vertices = 4 ) out;

in vec3 geomVelocity[];
in float geomMass[];

out float fragMass;

uniform bool useQuads;

const float size = 0.01;

void main(void){

    vec3 vel = geomVelocity[0];
    vec3 dir = normalize(vel);
    if (vel == vec3(0.0))
        dir = vec3(0.0, 1.0, 0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);

    // Calcula el producto cruz entre los vectores normalizados
    vec3 crossProduct = cross(dir, up);

    // Calcula el ángulo en radianes utilizando la función acos()
    float angleRadians = acos(dot(dir, up));

    // Ajusta el ángulo según la dirección de giro
    if (dot(crossProduct, vec3(0.0, 0.0, 1.0)) < 0.0) {
        angleRadians = -angleRadians;
    }

    // Convierte el ángulo a grados si lo deseas
    float angle = angleRadians;

    mat3 rot = mat3(
        cos(angle), -sin(angle), 0.0,
        sin(angle), cos(angle), 0.0,
        0.0, 0.0, 1.0
    );

    // vec4 dir_x = vec3(normalize(geom_acceleration[0]), 0.0);
    // rotate 90 degrees
    // vec4 dir_y = vec4(-dir_x.y, dir_x.x, 0.0, 0.0);
    if (useQuads)
    {
        vec3 offset = vec3(-1.0, 1.0, 0.0)*size;
        offset = rot*offset;
        vec4 vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec3(-1.0, -1.0, 0.0)*size;
        offset = rot*offset;
        vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec3(1.0, 1.0, 0.0)*size;
        offset = rot*offset;
        vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec3(1.0, -1.0, 0.0)*size;
        offset = rot*offset;
        vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();
    }
    else
    {
        vec3 offset = vec3(-1.0, -1.0, 0.0) * size;
        offset = rot*offset;
        vec4 vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec3(1.0, -1.0, 0.0) * size;
        offset = rot*offset;
        vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();

        offset = vec3(0.0, 1.0, 0.0) * size;
        offset = rot*offset;
        vertexPos = vec4(offset, 0.0) + gl_in[0].gl_Position;
        gl_Position = vertexPos;
        fragMass = geomMass[0];
        EmitVertex();
    }


    EndPrimitive();
}