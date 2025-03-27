#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;

class CGPUFence: public CBaseGPUObject<SDL_GPUFence>
{
  public:
	CGPUFence(std::shared_ptr<CGPUDevice> device, SDL_GPUFence* fence) : CBaseGPUObject(device, fence)
	{
	}
	~CGPUFence();

	void Wait() const;
    static void WaitForAll(std::shared_ptr<CGPUDevice> device, const CGPUFence** fences, u32 fenceCount);
};
