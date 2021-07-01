#version 330 core
out vec4 FragColor;

uniform vec3 point_color;

void main() {
    FragColor = vec4(point_color, 1.0f);
}