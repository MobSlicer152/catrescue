#include "SDL3/SDL_gpu.h"
#include "game/game.h"
#include "game/log.h"
#include "device.h"

#ifdef GAME_DEBUG
#define RENDER_DEBUG 1
#else
#define RENDER_DEBUG 0
#endif

CRenderDevice::CRenderDevice(cstr driverName)
{
    LogInfo("Creating render device%s%s", driverName ? " with driver " : "", driverName ? driverName : "");
    m_handle = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, RENDER_DEBUG, driverName);
    if (!m_handle)
    {
        LogError("Failed to create render device: %s", SDL_GetError());
        return;
    }
}

CRenderDevice::~CRenderDevice()
{
    if (IsGood())
    {
        SDL_DestroyGPUDevice(m_handle);
    }
}
