#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUCommandBuffer;

class CGPUDevice: public CBaseGPUObject<SDL_GPUDevice, CGPUDevice>
{
  public:
    CGPUDevice(cstr driverName = nullptr);
    ~CGPUDevice();
};
