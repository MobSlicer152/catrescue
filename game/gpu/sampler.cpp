#include "sampler.h"
#include "device.h"
#include "game/log.h"

CGPUSampler::CGPUSampler(std::shared_ptr<CGPUDevice> device, const SDL_GPUSamplerCreateInfo& info) : CBaseGPUObject(device)
{
    m_handle = SDL_CreateGPUSampler(m_parent->GetHandle(), &info);
    if (!m_handle)
    {
        LogError("Failed to create sampler: %s", SDL_GetError());
        return;
    }
}

CGPUSampler::~CGPUSampler()
{
    if (IsGood())
    {
        SDL_ReleaseGPUSampler(m_parent->GetHandle(), m_handle);
        m_handle = nullptr;
    }
}
