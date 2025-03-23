#version 420 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 color;

uniform mat4 model;
uniform mat4 camera; // proj * view

//uniform sampler2D diffuseTexture;
//uniform sampler2D normalMap;

out vec4 fragColor;

void main()
{
    gl_Position = camera * model * vec4(pos, 1.0);
    fragColor = color; // * texture(diffuseTexture, texCoord);
}
