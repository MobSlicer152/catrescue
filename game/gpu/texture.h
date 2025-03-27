#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUDevice;

class CGPUTexture: public CBaseGPUObject<SDL_GPUTexture>
{
  public:
	// pixels have to be either uint32 or float (for now)
	CGPUTexture(
		std::shared_ptr<CGPUDevice> device, SDL_GPUTextureFormat format,
		SDL_GPUTextureUsageFlags usage, u32 width, u32 height, void* pixels = nullptr);
	CGPUTexture(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr path, SDL_GPUTextureUsageFlags usage);
	~CGPUTexture();

	u32 GetWidth() const
	{
		return m_width;
	}

	u32 GetHeight() const
	{
		return m_height;
	}

	SDL_GPUTextureFormat GetFormat() const
	{
		return m_format;
	}

	SDL_GPUTextureUsageFlags GetUsage() const
	{
		return m_usage;
	}

  private:
	u32 m_width;
	u32 m_height;
	SDL_GPUTextureFormat m_format;
	SDL_GPUTextureUsageFlags m_usage;

	void Create();
	void Upload(void* pixels);
};
