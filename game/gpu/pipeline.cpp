#include "pipeline.h"
#include "device.h"
#include "game/log.h"
#include "game/macros.h"
#include "shader.h"
#include "shader_locations.h"
#include "texture.h"

CGPUGraphicsPipeline::CGPUGraphicsPipeline(std::shared_ptr<CGPUDevice> device, const GPUGraphicsPipelineCreateInfo_t& createInfo)
	: CBaseGPUObject(device)
{
	constexpr SDL_GPUVertexBufferDescription STANDARD_VERTEX_DESCRIPTIONS[] = {
		{0, sizeof(Vertex), SDL_GPU_VERTEXINPUTRATE_VERTEX}
    };
	constexpr SDL_GPUVertexAttribute STANDARD_VERTEX_ATTRIBUTES[] = {
		{STANDARD_VERTEX_POSITION, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Vertex, position)},
		{  STANDARD_VERTEX_NORMAL, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Vertex,   normal)},
		{STANDARD_VERTEX_TEXCOORD, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Vertex, texCoord)},
		{   STANDARD_VERTEX_COLOR, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, offsetof(Vertex,    color)},
	};

	static const SDL_GPUVertexInputState VERTEX_LAYOUTS[VertexType::VertexTypeCount] = {
		{STANDARD_VERTEX_DESCRIPTIONS, ARRAYSIZE(STANDARD_VERTEX_DESCRIPTIONS), STANDARD_VERTEX_ATTRIBUTES,
		 ARRAYSIZE(STANDARD_VERTEX_ATTRIBUTES)},
	};

	SDL_GPUGraphicsPipelineCreateInfo sdlInfo = {};
	sdlInfo.vertex_shader = createInfo.vertexShader.GetHandle();
	sdlInfo.fragment_shader = createInfo.fragmentShader.GetHandle();

	sdlInfo.rasterizer_state = createInfo.rasterState;
	sdlInfo.depth_stencil_state = createInfo.depthState;
	sdlInfo.vertex_input_state = VERTEX_LAYOUTS[createInfo.vertexType];
	sdlInfo.multisample_state.sample_count = createInfo.sampleCount;

    sdlInfo.target_info.color_target_descriptions = createInfo.colorTargets;
    sdlInfo.target_info.num_color_targets = createInfo.colorTargetCount;
    sdlInfo.target_info.depth_stencil_format = createInfo.depthFormat;
    sdlInfo.target_info.has_depth_stencil_target = createInfo.depthFormat != SDL_GPU_TEXTUREFORMAT_INVALID;

	m_handle = SDL_CreateGPUGraphicsPipeline(m_parent->GetHandle(), &sdlInfo);
    if (!m_handle)
    {
        LogError("Failed to create GPU graphics pipeline: %s", SDL_GetError());
        return;
    }
}

CGPUGraphicsPipeline::~CGPUGraphicsPipeline()
{
	if (IsGood())
	{
		SDL_ReleaseGPUGraphicsPipeline(m_parent->GetHandle(), m_handle);
		m_handle = nullptr;
	}
}
