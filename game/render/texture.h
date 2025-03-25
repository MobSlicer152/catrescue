#pragma once

#include "game/game.h"

class CGPUDevice;

class CGPUTexture
{
  public:
	CGPUTexture(
		const CGPUDevice* device, u32* pixels, u32 width, u32 height, SDL_GPUTextureFormat format,
		SDL_GPUTextureUsageFlags usage);
	~CGPUTexture();

	SDL_GPUTexture* GetHandle() const
	{
		return m_handle;
	}

	bool IsGood() const
	{
		return m_handle != nullptr;
	}

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
	const CGPUDevice* m_device;
	SDL_GPUTexture* m_handle;
	u32 m_width;
	u32 m_height;
	SDL_GPUTextureFormat m_format;
	SDL_GPUTextureUsageFlags m_usage;
};
