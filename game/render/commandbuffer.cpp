#include "commandbuffer.h"
#include "device.h"
#include "fence.h"

CGPUCommandBuffer::CGPUCommandBuffer(std::shared_ptr<CGPUDevice> device) : CBaseGPUObject(device, nullptr)
{
    m_handle = SDL_AcquireGPUCommandBuffer(m_parent->GetHandle());
}

std::shared_ptr<CGPUFence> CGPUCommandBuffer::Submit()
{
    auto fence = std::make_shared<CGPUFence>(m_parent, SDL_SubmitGPUCommandBufferAndAcquireFence(m_handle));
    m_handle = nullptr;
    return fence;
}
