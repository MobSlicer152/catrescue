#include "texture.h"
#include "commandbuffer.h"
#include "copypass.h"
#include "device.h"
#include "fence.h"
#include "game/log.h"
#include "game/texture.h"
#include "transferbuffer.h"

CGPUTexture::CGPUTexture(
	std::shared_ptr<CGPUDevice> device, SDL_GPUTextureFormat format, SDL_GPUTextureUsageFlags usage, u32 width, u32 height,
	void* pixels)
	: CBaseGPUObject(device), m_width(width), m_height(height), m_format(format), m_usage(usage)
{
	Create();
	if (pixels)
	{
		Upload(pixels);
	}
}

CGPUTexture::CGPUTexture(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr path, SDL_GPUTextureUsageFlags usage)
	: CBaseGPUObject(device), m_usage(usage)
{
	u32* pixels = LoadImage(storage, path, m_width, m_height, m_format);
	Create();
	if (pixels)
	{
		Upload(pixels);
	}
}

CGPUTexture::CGPUTexture(SDL_GPUTexture* handle, u32 width, u32 height, SDL_GPUTextureFormat m_format)
	: CBaseGPUObject(nullptr, handle), m_owned(false), m_width(width), m_height(height)
{
}

CGPUTexture::~CGPUTexture()
{
	if (m_owned && IsGood())
	{
		SDL_ReleaseGPUTexture(m_parent->GetHandle(), m_handle);
		m_handle = nullptr;
	}
}

void CGPUTexture::Create()
{
	m_owned = true;

	SDL_GPUTextureCreateInfo info = {};
	info.width = m_width;
	info.height = m_height;
	info.format = m_format;
	info.layer_count_or_depth = 1;
	info.num_levels = 1;
	info.usage = m_usage;
	m_handle = SDL_CreateGPUTexture(m_parent->GetHandle(), &info);
	if (!m_handle)
	{
		LogError("Failed to create GPU texture: %s", SDL_GetError());
		return;
	}
}

void CGPUTexture::Upload(void* pixels)
{
	auto cmdBuf = std::make_shared<CGPUCommandBuffer>(m_parent);

	u32 size = m_width * m_height * 4;

	CGPUTransferBuffer transferBuf(m_parent, size, SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD);
	void* mapping = transferBuf.Map();
	memcpy(mapping, pixels, size);
	transferBuf.Unmap();

	auto copyPass = std::make_shared<CGPUCopyPass>(cmdBuf);
	copyPass->UploadToTexture(*this, transferBuf, {m_width, m_height});
	copyPass->End();

	auto fence = cmdBuf->Submit();
	fence->Wait();
}
