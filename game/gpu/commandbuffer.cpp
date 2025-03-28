#include "commandbuffer.h"
#include "device.h"
#include "fence.h"

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
	SDL_WaitAndAcquireGPUSwapchainTexture(SDL_GPUCommandBuffer *command_buffer, SDL_Window *window, SDL_GPUTexture **swapchain_texture, Uint32 *swapchain_texture_width, Uint32 *swapchain_texture_height)
}
