#pragma once

#include "game/game.h"
#include "game/mesh.h"
#include "gpuobj.h"

class CGPUDevice;
class CGPUShader;

struct GPUGraphicsPipelineCreateInfo_t
{
	const CGPUShader& vertexShader;
	const CGPUShader& fragmentShader;
	VertexType vertexType;
	SDL_GPUPrimitiveType primitiveType;
	SDL_GPUSampleCount sampleCount;
    SDL_GPURasterizerState rasterState;
	SDL_GPUDepthStencilState depthState;
    const SDL_GPUColorTargetDescription* colorTargets;
    u32 colorTargetCount;
    SDL_GPUTextureFormat depthFormat;
};

class CGPUGraphicsPipeline: public CBaseGPUObject<SDL_GPUGraphicsPipeline>
{
  public:
	CGPUGraphicsPipeline(std::shared_ptr<CGPUDevice> device, const GPUGraphicsPipelineCreateInfo_t& createInfo);
    ~CGPUGraphicsPipeline();
};
