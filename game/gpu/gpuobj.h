#pragma once

#include "game/game.h"

class CGPUDevice;

template <typename H, typename P = CGPUDevice>
class CBaseGPUObject
{
  public:
    CBaseGPUObject() = default;
    CBaseGPUObject(std::shared_ptr<P> parent, H* handle = nullptr) : m_parent(parent), m_handle(handle)
    {
    }

    H* GetHandle() const
    {
        return m_handle;
    }

    bool IsGood() const
    {
        return m_handle != nullptr;
    }

  protected:
    std::shared_ptr<P> m_parent;
    H* m_handle;
};
