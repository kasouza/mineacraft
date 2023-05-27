#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec3 offset;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos + offset, 1.0);
}
