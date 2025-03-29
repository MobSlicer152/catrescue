#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;
class CGPUFence;
class CGPUTexture;
class CWindow;

class CGPUCommandBuffer: public CBaseGPUObject<SDL_GPUCommandBuffer>
{
  public:
	CGPUCommandBuffer(std::shared_ptr<CGPUDevice> device);
	~CGPUCommandBuffer()
    {
        // submitting a command buffer invalidates it
		if (IsGood())
		{
			Submit();
		}
    }

	std::shared_ptr<CGPUFence> Submit();

	std::shared_ptr<CGPUTexture> GetSwapChainTexture(std::shared_ptr<CWindow> window);

	void PushVertexUniform(u32 slot, const void* data, u32 size);
	void PushFragmentUniform(u32 slot, const void* data, u32 size);
};
