#include "shader.h"
#include "device.h"
#include "game/log.h"
#include "game/util.h"


CGPUShader::CGPUShader(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr name, SDL_GPUShaderStage stage) : CBaseGPUObject(device), m_stage(stage)
{
    constexpr cstr ENTRYPOINTS[] = {
        "VertexMain", // SDL_GPU_SHADERSTAGE_VERTEX
        "FragmentMain" // SDL_GPU_SHADERSTAGE_FRAGMENT
    };

    constexpr cstr STAGE_EXTENSIONS[] = {
        ".vs",
        ".ps"
    };

    cstr extension = ".spv";
    cstr folder = "vulkan";
    if (device->GetShaderFormat() & SDL_GPU_SHADERFORMAT_DXIL)
    {
        extension = ".cso";
        folder = "directx12";
    }

    dstr path = Format("shaders/%s/%s%s%s", folder, name, STAGE_EXTENSIONS[m_stage], extension);

	u64 size = 0;
	u8* shaderData = Read(storage, path, size);
	if (!shaderData)
	{
		return;
	}

    SDL_GPUShaderCreateInfo info = {};
    info.code = shaderData;
    info.code_size = size;
    info.stage = m_stage;
    info.num_uniform_buffers = 2;
    info.num_samplers = 1;
    info.entrypoint = ENTRYPOINTS[m_stage];
    info.format = device->GetShaderFormat();
	m_handle = SDL_CreateGPUShader(m_parent->GetHandle(), &info);
    if (!m_handle)
    {
        LogError("Failed to create shader %s: %s", name, SDL_GetError());
        return;
    }
}

CGPUShader::~CGPUShader()
{
    if (IsGood())
    {
        SDL_ReleaseGPUShader(m_parent->GetHandle(), m_handle);
        m_handle = 0;
    }
}
