#pragma once

#include "game.h"

struct Vertex_t
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec4 color;
};

typedef glm::ivec3 Index_t;