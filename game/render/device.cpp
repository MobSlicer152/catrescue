#include "device.h"
#include "commandbuffer.h"
#include "game/game.h"
#include "game/log.h"


#ifdef GAME_DEBUG
#define RENDER_DEBUG 1
#else
#define RENDER_DEBUG 0
#endif

CGPUDevice::CGPUDevice(cstr driverName) : CBaseGPUObject(nullptr, nullptr)
{
	LogInfo("Creating render device%s%s", driverName ? " with driver " : "", driverName ? driverName : "");
	m_handle = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, RENDER_DEBUG, driverName);
	if (!m_handle)
	{
		LogError("Failed to create render device: %s", SDL_GetError());
		return;
	}
}

CGPUDevice::~CGPUDevice()
{
	if (IsGood())
	{
		LogInfo("Destroying render device");
		SDL_DestroyGPUDevice(m_handle);
		m_handle = nullptr;
	}
}
