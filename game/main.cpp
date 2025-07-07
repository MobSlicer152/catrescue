#include "SDL3/SDL_main.h"

#include "game.h"
#include "gpu/buffer.h"
#include "gpu/commandbuffer.h"
#include "gpu/device.h"
#include "gpu/pipeline.h"
#include "gpu/renderpass.h"
#include "gpu/sampler.h"
#include "gpu/shader.h"
#include "gpu/shader_locations.h"
#include "gpu/texture.h"
#include "log.h"
#include "util.h"
#include "window.h"

int main(int argc, char* argv[])
{
	LogAddFile(fopen("output.log", "ab+"), LogLevelDebug);

	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	auto window = std::make_shared<CWindow>();
	auto device = std::make_shared<CGPUDevice>(window, argc > 1 ? argv[1] : nullptr);

	Vertex_t vertices[] = {
		{  {0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{ {0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
	};

	Index_t indices[] = {
		{0, 1, 2},
        {0, 2, 3}
    };

	auto vertexBuffer = std::make_shared<CGPUBuffer>(device, vertices, ARRAYSIZE(vertices));
	auto indexBuffer = std::make_shared<CGPUBuffer>(device, indices, ARRAYSIZE(indices));

	auto vertexShader = std::make_shared<CGPUShader>(device, storage, "main", SDL_GPU_SHADERSTAGE_VERTEX);
	auto fragmentShader = std::make_shared<CGPUShader>(device, storage, "main", SDL_GPU_SHADERSTAGE_FRAGMENT);

	SDL_GPUColorTargetDescription colorTarget = {
		device->GetSwapChainFormat(),
		{
			SDL_GPU_BLENDFACTOR_SRC_COLOR,
			SDL_GPU_BLENDFACTOR_DST_COLOR,
			SDL_GPU_BLENDOP_ADD,
			SDL_GPU_BLENDFACTOR_SRC_ALPHA,
			SDL_GPU_BLENDFACTOR_DST_ALPHA,
			SDL_GPU_BLENDOP_ADD,
			SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G | SDL_GPU_COLORCOMPONENT_B | SDL_GPU_COLORCOMPONENT_A,
			true,
			true
		},
	};

	GPUGraphicsPipelineCreateInfo_t pipelineInfo = {
		*vertexShader,
		*fragmentShader,
		StandardVertex,
		SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		SDL_GPU_SAMPLECOUNT_1,
		{
			SDL_GPU_FILLMODE_FILL,
			SDL_GPU_CULLMODE_BACK,
			SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
		},
		{
			SDL_GPU_COMPAREOP_LESS,
			{
				SDL_GPU_STENCILOP_KEEP,
				SDL_GPU_STENCILOP_KEEP
			},
			{
				SDL_GPU_STENCILOP_KEEP,
				SDL_GPU_STENCILOP_KEEP
			},
			0x00,
			0x00,
			true,
			true,
			false,
		},
		&colorTarget,
		1,
		SDL_GPU_TEXTUREFORMAT_INVALID
	};
	auto pipeline = std::make_shared<CGPUGraphicsPipeline>(device, pipelineInfo);
	vertexShader.reset();
	fragmentShader.reset();

	SDL_GPUSamplerCreateInfo samplerInfo = {};
	samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
	samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
	samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
	samplerInfo.min_lod = 0.0f;
	samplerInfo.max_lod = 1.0f;
	samplerInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
	samplerInfo.min_filter = SDL_GPU_FILTER_NEAREST;
	auto sampler = std::make_shared<CGPUSampler>(device, samplerInfo);

	auto texture = std::make_shared<CGPUTexture>(device, storage, "textures/missing.qoi", SDL_GPU_TEXTUREUSAGE_SAMPLER);

	struct
	{
		glm::mat4 view;
		glm::mat4 projection;
	} sceneUbo;

	sceneUbo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	sceneUbo.projection = glm::perspective(glm::radians(90.0f), window->GetAspect(), 0.1f, 1000.0f);

	struct
	{
		glm::mat4 model;
	} objectUbo;

	objectUbo.model = glm::mat4(1.0f);

	u64 now = 0;
	u64 last = now;
	while (window->IsOpen())
	{
		now = SDL_GetTicks();
		f32 delta = (now - last) / 1000.0f;

		window->Update();
		if (window->Resized())
		{
			sceneUbo.projection = glm::perspective(glm::radians(90.0f), window->GetAspect(), 0.1f, 1000.0f);
		}

		//objectUbo.model = glm::rotate(objectUbo.model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		auto cmdBuf = std::make_shared<CGPUCommandBuffer>(device);
		auto colorTarget = cmdBuf->GetSwapChainTexture(window);
		if (colorTarget)
		{
			CGPURenderPass renderPass(cmdBuf, colorTarget, nullptr, {0.2f, 0.2f, 0.2f, 0.2f});

			renderPass.BindVertexBuffer(vertexBuffer);
			renderPass.BindIndexBuffer(indexBuffer);
			
			renderPass.BindFragmentSampler(texture, sampler);

			cmdBuf->PushVertexUniform(SCENE_UBO_LOCATION, &sceneUbo, sizeof(sceneUbo));
			cmdBuf->PushVertexUniform(OBJECT_UBO_LOCATION, &objectUbo, sizeof(objectUbo));
			
			renderPass.BindGraphicsPipeline(pipeline);

			renderPass.DrawIndexed(indexBuffer->GetIndexCount());

			renderPass.End();
		}

		cmdBuf->Submit();

		world.progress(delta);

		last = now;
	}

	texture.reset();
	sampler.reset();
	pipeline.reset();
	device.reset();
	window.reset();

	return 0;
}
