#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUCommandBuffer;

class CGPUDevice: public CBaseGPUObject<SDL_GPUDevice, CWindow>
{
  public:
	CGPUDevice(std::shared_ptr<CWindow> window, cstr driverName = nullptr);
	~CGPUDevice();

	SDL_GPUTextureFormat GetSwapChainFormat() const
	{
		return m_swapChainFormat;
	}

	SDL_GPUShaderFormat GetShaderFormat() const
	{
		return m_shaderFormat;
	}

  private:
	SDL_GPUTextureFormat m_swapChainFormat;
	SDL_GPUShaderFormat m_shaderFormat;
};
