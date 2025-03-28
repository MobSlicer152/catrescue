#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;

class CGPUSampler: public CBaseGPUObject<SDL_GPUSampler>
{
  public:
    CGPUSampler(std::shared_ptr<CGPUDevice> device, const SDL_GPUSamplerCreateInfo& info);
    ~CGPUSampler();
};
