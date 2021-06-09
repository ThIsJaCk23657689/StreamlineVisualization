#version 330 core
layout (location = 0) in vec2 vertex;

uniform mat4 model;
uniform mat4 projection;
uniform float point_size;

void main() {
    gl_PointSize = point_size;
    gl_Position = projection * model * vec4(vertex, 0.0f, 1.0f);
}