#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;
class CGPUFence;

class CGPUCommandBuffer: public CBaseGPUObject<SDL_GPUCommandBuffer>
{
  public:
	CGPUCommandBuffer(std::shared_ptr<CGPUDevice> device);
	~CGPUCommandBuffer()
    {
        // submitting a command buffer invalidates it
        m_handle = nullptr;
    }

	std::shared_ptr<CGPUFence> Submit();
};
