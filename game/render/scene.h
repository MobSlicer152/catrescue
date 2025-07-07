#pragma once

#include "game.h"

struct SceneUBO
{
	glm::mat4 view;
	glm::mat4 projection;
};

struct ObjectUBO
{
    glm::mat4 model;
};
