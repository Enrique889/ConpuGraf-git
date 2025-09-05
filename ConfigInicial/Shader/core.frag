#version 330 core
out vec4 color;

// color uniforme por cubo
uniform vec4 uColor;

void main() {
    color = uColor;
}
