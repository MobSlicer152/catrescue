#pragma once

#include "game.h"

struct EntityPosition
{
	glm::vec3 value;
};

struct EntityRotation
{
	glm::quat value;
};

struct EntityScale
{
	glm::vec3 value;
};

FORCEINLINE glm::mat4 CalculateTransform(const EntityPosition& position, const EntityRotation& rotation, const EntityScale& scale)
{
	glm::mat4 t = glm::identity<glm::mat4>();
	t = glm::translate(t, position.value);
	t *= glm::mat4_cast(rotation.value);
	t = glm::scale(t, scale.value);
	return t;
}
