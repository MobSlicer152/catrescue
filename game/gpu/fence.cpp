#include "device.h"
#include "fence.h"

CGPUFence::~CGPUFence()
{
    if (IsGood())
    {
        SDL_ReleaseGPUFence(m_parent->GetHandle(), m_handle);
        m_handle = nullptr;
    }
}

void CGPUFence::Wait() const
{
    const CGPUFence* fences[] = {this};
    WaitForAll(m_parent, fences, 1);
}

void CGPUFence::WaitForAll(std::shared_ptr<CGPUDevice> device, const CGPUFence **fences, u32 fenceCount)
{
    std::vector<SDL_GPUFence*> fenceHandles(fenceCount);
    for (u32 i = 0; i < fenceCount; i++)
    {
        fenceHandles[i] = fences[i]->GetHandle();
    }
    SDL_WaitForGPUFences(device->GetHandle(), true, fenceHandles.data(), (u32)fenceHandles.size());
}
