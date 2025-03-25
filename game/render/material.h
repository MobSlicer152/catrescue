#pragma once

#include "game/game.h"
#include "shader.h"
#include "texture.h"

class IMaterial
{
  public:
    virtual ~IMaterial() = default;

    virtual const IShaderProgram* GetShader() const = 0;
    virtual const ITexture* GetTexture() const = 0;
};

extern IMaterial* CreateMaterial();
