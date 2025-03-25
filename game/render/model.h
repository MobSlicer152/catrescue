#pragma once

#include "game/game.h"
#include "material.h"

class IModel
{
  public:
    virtual ~IModel() = default;

    virtual void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj) = 0;
    virtual const IMaterial* GetMaterial() const = 0;
};
