#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUCommandBuffer;
class CGPUBuffer;
class CGPUGraphicsPipeline;
class CGPUSampler;
class CGPUTexture;

class CGPURenderPass: public CBaseGPUObject<SDL_GPURenderPass, CGPUCommandBuffer>
{
  public:
	CGPURenderPass(
		std::shared_ptr<CGPUCommandBuffer> cmdBuf, const CGPUTexture* colorTargets, u32 colorTargetCount,
		const CGPUTexture* depthTarget, glm::vec4 clearColor = {0, 0, 0, 0}, f32 clearDepth = 1.0f);
	CGPURenderPass(
		std::shared_ptr<CGPUCommandBuffer> cmdBuf, const CGPUTexture& colorTarget, const CGPUTexture* depthTarget,
		glm::vec4 clearColor = {0, 0, 0, 0}, f32 clearDepth = 1.0f)
		: CGPURenderPass(cmdBuf, &colorTarget, 1, depthTarget, clearColor)
	{
	}
	~CGPURenderPass()
    {
        if (IsGood())
        {
            End();
        }
    }

    void End();

	void BindVertexBuffers(const CGPUBuffer* buffers, u32 bufferCount, const u32* offsets = nullptr, u32 offsetCount = 0, u32 firstSlot = 0);
	void BindVertexBuffer(const CGPUBuffer& buffer, u32 offset = 0, u32 slot = 0)
	{
		BindVertexBuffers(&buffer, 1, &offset, 1, slot);
	}

	void BindIndexBuffer(const CGPUBuffer& buffer, u32 offset = 0);

	void BindFragmentSamplers(const CGPUTexture* textures, const CGPUSampler* samplers, u32 count);
	void BindFragmentSampler(const CGPUTexture& texture, const CGPUSampler& sampler)
	{
		BindFragmentSamplers(&texture, &sampler, 1);
	}

	void BindGraphicsPipeline(const CGPUGraphicsPipeline& pipeline);

	void DrawIndexed(u32 indexCount, u32 instanceCount = 1);
	void DrawIndexedIndirect(const CGPUBuffer& params, u32 drawCount = 1, u32 offset = 0);
};
