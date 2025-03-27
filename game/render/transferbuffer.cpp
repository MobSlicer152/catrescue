#include "device.h"
#include "transferbuffer.h"
#include "game/log.h"

CGPUTransferBuffer::CGPUTransferBuffer(std::shared_ptr<CGPUDevice> device, u32 size, SDL_GPUTransferBufferUsage usage)
	: CBaseGPUObject(device, nullptr), m_usage(usage)
{
    SDL_GPUTransferBufferCreateInfo info = {};
    info.size = size;
    info.usage = m_usage;
    m_handle = SDL_CreateGPUTransferBuffer(m_parent->GetHandle(), &info);
    if (!m_handle)
    {
        LogError("Failed to create %u-byte GPU transfer buffer: %s", size, SDL_GetError());
        return;
    }
}

CGPUTransferBuffer::~CGPUTransferBuffer()
{
    if (IsGood())
    {
        SDL_ReleaseGPUTransferBuffer(m_parent->GetHandle(), m_handle);
    }
}

void* CGPUTransferBuffer::Map()
{
    return SDL_MapGPUTransferBuffer(m_parent->GetHandle(), m_handle, false);
}

void CGPUTransferBuffer::Unmap()
{
    SDL_UnmapGPUTransferBuffer(m_parent->GetHandle(), m_handle);
}
