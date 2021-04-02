#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightpos;

out vec2 light;
out vec2 coord;

void main() {
    gl_Position = projection * view * vec4(aPos.y, aPos.x, 0.0f, 1.0f);
    light = lightpos.xy;
    coord = vec2(aPos.y, aPos.x);
}