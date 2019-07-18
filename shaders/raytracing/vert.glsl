#version 330 core

layout (location = 0) in vec2 position;

out vec3 RayDirection;

struct Camera {
    vec3 direction, up, right;
    vec3 position;
};

uniform Camera camera;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    RayDirection = camera.direction + position.x * camera.right + position.y * camera.up;
}