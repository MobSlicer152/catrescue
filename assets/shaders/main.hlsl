// Code is derived from various files in https://github.com/SaschaWillems/Vulkan/tree/master/shaders

#include "common.hlsli"

cbuffer UBO : register(SCENE_UBO_LOCATION, space1)
{
    float4x4 view;
    float4x4 projection;
}

cbuffer OBJECT_UBO : register(OBJECT_UBO_LOCATION, space1)
{
    float4x4 model;
}

struct VERTEX_INPUT
{
    float3 position : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 textureCoordinate : TEXCOORD2;
    float4 colour : TEXCOORD3;
};

struct VERTEX_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float2 textureCoordinate : TEXCOORD1;
    float4 colour : TEXCOORD2;
};

VERTEX_OUTPUT VertexMain(VERTEX_INPUT input)
{
    VERTEX_OUTPUT output;

    float4x4 MVP = mul(projection, mul(view, model));
    output.position = mul(float4(input.position, 1.0), MVP);
    output.normal = input.normal;
    output.textureCoordinate = input.textureCoordinate;
    output.colour = input.colour;

    return output;
}

Texture2D texture : register(t0, space2);
SamplerState textureSampler : register(s0, space2);

float4 FragmentMain(VERTEX_OUTPUT input) : SV_TARGET
{
    return texture.Sample(textureSampler, input.textureCoordinate) * input.colour;
}