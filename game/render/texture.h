#pragma once

#include "game/game.h"

class ITexture
{
  public:
    virtual ~ITexture() = default;

    virtual void Bind() = 0;
    virtual u32 GetWidth() = 0;
    virtual u32 GetHeight() = 0;
};
