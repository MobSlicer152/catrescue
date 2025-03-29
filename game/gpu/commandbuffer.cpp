#include "commandbuffer.h"
#include "device.h"
#include "fence.h"
#include "texture.h"
#include "game/log.h"
#include "game/window.h"

CGPUCommandBuffer::CGPUCommandBuffer(std::shared_ptr<CGPUDevice> device) : CBaseGPUObject(device)
{
    m_handle = SDL_AcquireGPUCommandBuffer(m_parent->GetHandle());
}

std::shared_ptr<CGPUFence> CGPUCommandBuffer::Submit()
{
    auto fence = std::make_shared<CGPUFence>(m_parent, SDL_SubmitGPUCommandBufferAndAcquireFence(m_handle));
    m_handle = nullptr;
    return fence;
}

std::shared_ptr<CGPUTexture> CGPUCommandBuffer::GetSwapChainTexture(std::shared_ptr<CWindow> window)
{
	SDL_GPUTexture* texture;
	u32 width = 0;
	u32 height = 0;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_handle, window->GetHandle(), &texture, &width, &height))
	{
		LogError("Failed to acquire swap chain texture: %s", SDL_GetError());
		return nullptr;
	}

	// if the window is minimized, it succeeds but gives a null texture
	if (texture)
	{
		return std::make_shared<CGPUTexture>(texture, width, height, m_parent->GetSwapChainFormat());
	}

	return nullptr;
}

void CGPUCommandBuffer::PushVertexUniform(u32 slot, const void* data, u32 size)
{
	SDL_PushGPUVertexUniformData(m_handle, slot, data, size);
}

void CGPUCommandBuffer::PushFragmentUniform(u32 slot, const void* data, u32 size)
{
	SDL_PushGPUFragmentUniformData(m_handle, slot, data, size);
}
