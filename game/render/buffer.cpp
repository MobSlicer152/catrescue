#include "buffer.h"
#include "commandbuffer.h"
#include "copypass.h"
#include "device.h"
#include "fence.h"
#include "game/log.h"
#include "transferbuffer.h"

CGPUBuffer::CGPUBuffer(std::shared_ptr<CGPUDevice> device, SDL_GPUBufferUsageFlags usage, const void* data, u32 size)
	: CBaseGPUObject(device, nullptr), m_usage(usage)
{
	SDL_GPUBufferCreateInfo info = {};
	info.usage = m_usage;
	info.size = size;
	m_handle = SDL_CreateGPUBuffer(m_parent->GetHandle(), &info);
	if (!m_handle)
	{
		LogError("Failed to create GPU buffer: %s", SDL_GetError());
		return;
	}

	auto cmdBuf = std::make_shared<CGPUCommandBuffer>(m_parent);

	CGPUTransferBuffer transferBuf(m_parent, size, SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD);
	void* mapping = transferBuf.Map();
	memcpy(mapping, data, size);
	transferBuf.Unmap();

	auto copyPass = std::make_shared<CGPUCopyPass>(cmdBuf);
	copyPass->UploadToBuffer(*this, transferBuf, size);
    copyPass->End();

	auto fence = cmdBuf->Submit();
	fence->Wait();
}

CGPUBuffer::~CGPUBuffer()
{
	if (IsGood())
	{
		SDL_ReleaseGPUBuffer(m_parent->GetHandle(), m_handle);
		m_handle = nullptr;
	}
}
