#pragma once

#include "game/game.h"

class CGPUDevice
{
  public:
    CGPUDevice(cstr driverName = nullptr);
    ~CGPUDevice();

    SDL_GPUDevice* GetHandle() const
    {
        return m_handle;
    }

    bool IsGood() const
    {
        return m_handle != nullptr;
    }

  private:
    SDL_GPUDevice* m_handle;
};
