#include "device.h"
#include "commandbuffer.h"
#include "game/game.h"
#include "game/log.h"
#include "game/window.h"

#ifdef GAME_DEBUG
#define RENDER_DEBUG 1
#else
#define RENDER_DEBUG 0
#endif

CGPUDevice::CGPUDevice(std::shared_ptr<CWindow> window, cstr driverName) : CBaseGPUObject(window)
{
	LogInfo("Creating render device%s%s", driverName ? " with driver " : "", driverName ? driverName : "");
	m_handle = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, RENDER_DEBUG, driverName);
	if (!m_handle)
	{
		LogError("Failed to create render device: %s", SDL_GetError());
		return;
	}

	if (!SDL_ClaimWindowForGPUDevice(m_handle, m_parent->GetHandle()))
	{
		LogError("Failed to claim window for render device: %s", SDL_GetError());
		SDL_DestroyGPUDevice(m_handle);
		m_handle = nullptr;
		return;
	}

	m_swapChainFormat = SDL_GetGPUSwapchainTextureFormat(m_handle, m_parent->GetHandle());
	m_shaderFormat = SDL_GetGPUShaderFormats(m_handle);
}

CGPUDevice::~CGPUDevice()
{
	if (IsGood())
	{
		LogInfo("Destroying render device");
		SDL_ReleaseWindowFromGPUDevice(m_handle, m_parent->GetHandle());
		SDL_DestroyGPUDevice(m_handle);
		m_handle = nullptr;
	}
}
