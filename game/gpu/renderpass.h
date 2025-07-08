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
		std::shared_ptr<CGPUCommandBuffer> cmdBuf, const std::vector<std::shared_ptr<CGPUTexture>>& colorTargets,
		const std::shared_ptr<CGPUTexture> depthTarget, glm::vec4 clearColor = {0, 0, 0, 0}, f32 clearDepth = 1.0f);
	CGPURenderPass(
		std::shared_ptr<CGPUCommandBuffer> cmdBuf, const std::shared_ptr<CGPUTexture> colorTarget,
		const std::shared_ptr<CGPUTexture> depthTarget, glm::vec4 clearColor = {0, 0, 0, 0}, f32 clearDepth = 1.0f)
		: CGPURenderPass(cmdBuf, std::vector {colorTarget}, depthTarget, clearColor)
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

	void BindVertexBuffers(
		const std::vector<std::shared_ptr<CGPUBuffer>>& buffers, const std::vector<u32>& offsets, u32 firstSlot = 0);
	void BindVertexBuffer(const std::shared_ptr<CGPUBuffer> buffer, u32 offset = 0, u32 slot = 0)
	{
		BindVertexBuffers({buffer}, {offset}, slot);
	}

	void BindIndexBuffer(const std::shared_ptr<CGPUBuffer> buffer, u32 offset = 0);

	void BindFragmentSamplers(
		const std::vector<std::shared_ptr<CGPUTexture>>& textures, const std::vector<std::shared_ptr<CGPUSampler>>& samplers, u32 firstSlot = 0);
	void BindFragmentSampler(
		const std::shared_ptr<CGPUTexture> texture, const std::shared_ptr<CGPUSampler> sampler, u32 firstSlot = 0)
	{
		BindFragmentSamplers({texture}, {sampler}, firstSlot);
	}

	void BindGraphicsPipeline(const CGPUGraphicsPipeline& pipeline);
	void BindGraphicsPipeline(const std::shared_ptr<CGPUGraphicsPipeline> pipeline);

	void DrawIndexed(u32 indexCount, u32 instanceCount = 1);
	void DrawIndexedIndirect(const std::shared_ptr<CGPUBuffer> params, u32 drawCount = 1, u32 offset = 0);
};
