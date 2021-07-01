#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in float trail_index;
layout (location = 2) in float point_index;

uniform mat4 model;
uniform mat4 projection;
// uniform float point_size;

void main() {
    float point_size = 1 + point_index * 0.02;
    if (point_size > 3.0f) point_size = 3.0f;
    gl_PointSize = point_size;

    gl_Position = projection * model * vec4(position, 0.0f, 1.0f);
}