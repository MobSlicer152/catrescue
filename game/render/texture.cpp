#include "texture.h"
#include "device.h"
#include "game/log.h"

CGPUTexture::CGPUTexture(
	std::shared_ptr<CGPUDevice> device, u32* pixels, u32 width, u32 height, SDL_GPUTextureFormat format, SDL_GPUTextureUsageFlags usage)
	: CBaseGPUObject(device, nullptr), m_width(width), m_height(height), m_format(format), m_usage(usage)
{
	LogInfo("Creating GPU %ux%u texture with format %u and usage 0b%08b", m_width, m_height, m_format, m_usage);

	SDL_GPUTextureCreateInfo info = {};
	info.width = width;
	info.height = height;
	info.format = format;
	info.layer_count_or_depth = 1;
	info.num_levels = 1;
	info.usage = SDL_GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ;
	m_handle = SDL_CreateGPUTexture(m_parent->GetHandle(), &info);
	if (!m_handle)
	{
		LogError("Failed to create GPU texture: %s", SDL_GetError());
        return;
	}
}

CGPUTexture::~CGPUTexture()
{
    if (IsGood())
    {
        SDL_ReleaseGPUTexture(m_parent->GetHandle(), m_handle);
		m_handle = nullptr;
    }
}
