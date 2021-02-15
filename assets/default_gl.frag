#version 330 core
out vec4 Pixel;
in vec4 Shade;

void main() {
    Pixel = Shade;
}