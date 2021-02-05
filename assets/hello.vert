#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

layout (location = 0) out vec3 Shading;

layout (push_constant) uniform constants {
    mat4 transform;
} PushConstants;

vec3 colors[3] = vec3[] (
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = PushConstants.transform * vec4(Position.xyz, 1.0);
    Shading = vec4(Color.xyz, 1.0);
}