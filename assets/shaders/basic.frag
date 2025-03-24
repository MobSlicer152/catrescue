#version 420 core

in struct {
    vec4 color;
    vec2 texCoord;
} fragIn;

uniform sampler2D diffuse;
//uniform sampler2D normalMap;

out vec4 outColor;

void main() {
    //vec3 normal = texture(normalMap, fragIn.texCoord).rgb;

    outColor = texture(diffuse, vec2(fragIn.texCoord.x, 1.0 - fragIn.texCoord.y)) * fragIn.color;
}
