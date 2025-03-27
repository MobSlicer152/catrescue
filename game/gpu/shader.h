#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUShader: public CBaseGPUObject<SDL_GPUShader>
{
  public:
    CGPUShader();
    ~CGPUShader();
};
