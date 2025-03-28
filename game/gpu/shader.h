#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;

class CGPUShader: public CBaseGPUObject<SDL_GPUShader>
{
  public:
    CGPUShader(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr name, SDL_GPUShaderStage stage);
    ~CGPUShader();

    SDL_GPUShaderStage GetStage() const
    {
        return m_stage;
    }

  private:
    SDL_GPUShaderStage m_stage;
};
