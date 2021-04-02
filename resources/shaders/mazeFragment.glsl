#version 330 core

in vec2 light;
in vec2 coord;

out vec4 FragColor;

void main() {
    float constant = 1.0f;
    float linear = 8.00f;
    float quadratic = 4.00f;
    float distance  = length(light - coord);
    float attenuation = 1.0 / (constant + linear * distance);
    FragColor = vec4(0.0f, 1.0f, 1.0f, attenuation * 1.0f);
}
