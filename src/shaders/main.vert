in vec3 position;
in vec3 velocity;
in vec3 acceleration;

out vec3 f_acceleration;

void main()
{
    vec3 pos = position + velocity * time + acceleration * time * time;
    gl_Position = vec4(pos, 1.0);
}
