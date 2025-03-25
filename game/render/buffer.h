#pragma once

#include "game/game.h"

class CGPUBuffer
{
  public:
    CGPUBuffer(SDL_GPUBufferUsageFlags usage);
    ~CGPUBuffer();

    bool IsVertexBuffer() const
    {
        return m_usage & SDL_GPU_BUFFERUSAGE_VERTEX;
    }

    bool IsIndexBuffer() const
    {
        return m_usage & SDL_GPU_BUFFERUSAGE_VERTEX;
    }

    SDL_GPUBufferUsageFlags GetUsage() const
    {
        return m_usage;
    }

  protected:
    SDL_GPUBuffer* m_handle;
    SDL_GPUBufferUsageFlags m_usage;
};
