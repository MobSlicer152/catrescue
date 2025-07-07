#pragma once

#include "game.h"

enum VertexType
{
	StandardVertex, // Vertex_t
	VertexTypeCount
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec4 color;
};

typedef glm::ivec3 Index;