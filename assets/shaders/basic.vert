#version 420 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out struct {
    vec4 color;
    vec2 texCoord;
} fragIn;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0);
    fragIn.color = color;
    fragIn.texCoord = texCoord;
}
