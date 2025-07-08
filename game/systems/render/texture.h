#pragma once

#include "game.h"

#include "gpu/device.h"
#include "gpu/renderpass.h"
#include "gpu/sampler.h"
#include "gpu/texture.h"

class CTexture
{
  public:
	CTexture(std::shared_ptr<CGPUSampler> sampler, std::shared_ptr<CGPUTexture> texture) : m_sampler(sampler), m_handle(texture)
	{
	}

	CTexture(
		std::shared_ptr<CGPUDevice> device, std::shared_ptr<CGPUSampler> sampler, SDL_Storage* storage, cstr path,
		SDL_GPUTextureUsageFlags usage)
		: m_sampler(sampler), m_handle(std::make_shared<CGPUTexture>(device, storage, path, usage))
	{
	}

    void Bind(CGPURenderPass& renderPass, u32 slot = 0) const
    {
        renderPass.BindFragmentSampler(m_handle, m_sampler, slot);
    }

	std::shared_ptr<CGPUSampler> GetSampler() const
	{
		return m_sampler;
	}

	std::shared_ptr<CGPUTexture> GetHandle() const
	{
		return m_handle;
	}

  private:
	std::shared_ptr<CGPUSampler> m_sampler;
	std::shared_ptr<CGPUTexture> m_handle;
};
