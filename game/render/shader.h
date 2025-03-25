#pragma once

#include "game/game.h"

class IShaderProgram
{
  public:
    virtual ~IShaderProgram() = default;

    virtual void Bind() const = 0;
    
	virtual void SetUniform(cstr name, glm::vec2 value) const = 0;
	virtual void SetUniform(cstr name, glm::vec3 value) const = 0;
	virtual void SetUniform(cstr name, glm::vec4 value) const = 0;
	virtual void SetUniform(cstr name, glm::mat4 value) const = 0;
};
