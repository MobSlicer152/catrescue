#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;

class CGPUTransferBuffer: public CBaseGPUObject<SDL_GPUTransferBuffer>
{
  public:
    CGPUTransferBuffer(std::shared_ptr<CGPUDevice> device, u32 size, SDL_GPUTransferBufferUsage usage);
    ~CGPUTransferBuffer();

    void* Map();
    void Unmap();

    SDL_GPUTransferBufferUsage GetUsage() const
    {
        return m_usage;
    }

  private:
    SDL_GPUTransferBufferUsage m_usage;
};
