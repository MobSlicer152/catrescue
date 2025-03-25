#pragma once

#include "game/game.h"

class CRenderDevice
{
  public:
    CRenderDevice(cstr driverName = nullptr);
    ~CRenderDevice();

    SDL_GPUDevice* GetDevice() const
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
