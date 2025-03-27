#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUBuffer;
class CGPUDevice;
class CGPUCommandBuffer;
class CGPUTexture;
class CGPUTransferBuffer;

class CGPUCopyPass: public CBaseGPUObject<SDL_GPUCopyPass, CGPUCommandBuffer>
{
  public:
	CGPUCopyPass(std::shared_ptr<CGPUCommandBuffer> cmdBuf);
	~CGPUCopyPass()
    {
        if (IsGood())
        {
            End();
        }
    }

    void End();

	void UploadToBuffer(const CGPUBuffer& dest, const CGPUTransferBuffer& src, u32 size, u32 srcOffset = 0, u32 destOffset = 0);
	void UploadToTexture(
		const CGPUTexture& texture, const CGPUTransferBuffer& src, glm::ivec2 size, glm::ivec2 srcPos = {0, 0},
		glm::ivec2 destPos = {0, 0}, u32 destMip = 0, u32 destLayer = 0);
};
