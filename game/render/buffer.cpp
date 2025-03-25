#include "buffer.h"
#include "device.h"
#include "game/log.h"

CGPUBuffer::CGPUBuffer(const CGPUDevice* device, SDL_GPUBufferUsageFlags usage, const void* data, u32 size) : m_device(device), m_handle(nullptr), m_usage(usage)
{
    LogInfo("Creating %u-byte GPU buffer with usage 0b%08b", size, m_usage);

    SDL_GPUBufferCreateInfo info = {};
    info.usage = m_usage;
    info.size = size;
    m_handle = SDL_CreateGPUBuffer(m_device->GetHandle(), &info);
    if (!m_handle)
    {
        LogError("Failed to create GPU buffer: %s", SDL_GetError());
        return;
    }
}

CGPUBuffer::~CGPUBuffer()
{
    if (IsGood())
    {
        SDL_ReleaseGPUBuffer(m_device->GetHandle(), m_handle);
    }
}
