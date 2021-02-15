#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
out vec4 Shade;

void main() {
    gl_Position = vec4(Position.xyz, 1.0);
    Shade = vec4(Color.xyz, 1);
}