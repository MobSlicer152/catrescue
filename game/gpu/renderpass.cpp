#include "renderpass.h"
#include "buffer.h"
#include "commandbuffer.h"
#include "gpuobj.h"
#include "texture.h"

CGPURenderPass::CGPURenderPass(
	std::shared_ptr<CGPUCommandBuffer> cmdBuf, const CGPUTexture* colorTargets, u32 colorTargetCount,
	const CGPUTexture* depthTarget, glm::vec4 clearColor, f32 clearDepth)
	: CBaseGPUObject(cmdBuf)
{
	std::vector<SDL_GPUColorTargetInfo> colorInfos(colorTargetCount);
	for (usize i = 0; i < colorInfos.size(); i++)
	{
		auto& info = colorInfos[i];
		info.texture = colorTargets[i].GetHandle();
		info.mip_level = 0;
		info.layer_or_depth_plane = 0;

		info.load_op = SDL_GPU_LOADOP_CLEAR;
		info.clear_color.r = clearColor.r;
		info.clear_color.b = clearColor.g;
		info.clear_color.b = clearColor.b;
		info.clear_color.a = clearColor.a;
	}

	SDL_GPUDepthStencilTargetInfo depthInfo = {};
	if (depthTarget)
	{
		depthInfo.texture = depthTarget->GetHandle();
		depthInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		depthInfo.clear_depth = clearDepth;
	}

	m_handle = SDL_BeginGPURenderPass(
		m_parent->GetHandle(), colorInfos.data(), (u32)colorInfos.size(), depthTarget ? &depthInfo : nullptr);
}

void CGPURenderPass::End()
{
	SDL_EndGPURenderPass(m_handle);
}

void CGPURenderPass::BindVertexBuffers(
	const CGPUBuffer* buffers, u32 bufferCount, const u32* offsets, u32 offsetCount, u32 firstSlot)
{
	std::vector<SDL_GPUBufferBinding> bindings(bufferCount);
	for (usize i = 0; i < bindings.size(); i++)
	{
		bindings[i].buffer = buffers[i].GetHandle();
		if (offsets && i < offsetCount)
		{
			bindings[i].offset = offsets[i];
		}
	}

	SDL_BindGPUVertexBuffers(m_handle, firstSlot, bindings.data(), (u32)bindings.size());
}

void CGPURenderPass::BindIndexBuffer(const CGPUBuffer& buffer, u32 offset)
{
	SDL_GPUBufferBinding binding = {};
	binding.buffer = buffer.GetHandle();
	binding.offset = offset;
	SDL_BindGPUIndexBuffer(m_handle, &binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
}

void CGPURenderPass::BindFragmentSamplers(const CGPUTexture* textures, const CGPUSampler* samplers, u32 count)
{
}

void CGPURenderPass::BindGraphicsPipeline(const CGPUGraphicsPipeline& pipeline)
{
}

void CGPURenderPass::DrawIndexed(u32 indexCount, u32 instanceCount)
{
	SDL_DrawGPUIndexedPrimitives(m_handle, indexCount, instanceCount, 0, 0, 0);
}

void CGPURenderPass::DrawIndexedIndirect(const CGPUBuffer& params, u32 drawCount, u32 offset)
{
	SDL_DrawGPUIndexedPrimitivesIndirect(m_handle, params.GetHandle(), offset, drawCount);
}