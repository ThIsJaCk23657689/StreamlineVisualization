#version 330 core
out vec4 FragColor;

uniform vec3 sprite_color;

void main() {
    FragColor = vec4(sprite_color, 1.0f);
}