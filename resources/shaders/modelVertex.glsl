#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 view;

void main() {
    gl_Position = view * model * vec4(aPos, 0.5f, 1.0f);
}
