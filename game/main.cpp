#include "SDL3/SDL_main.h"

#include "game.h"
#include "gpu/buffer.h"
#include "gpu/commandbuffer.h"
#include "gpu/device.h"
#include "gpu/pipeline.h"
#include "gpu/renderpass.h"
#include "gpu/shader.h"
#include "gpu/texture.h"
#include "log.h"
#include "util.h"
#include "window.h"

int SDL_main(int argc, char* argv[])
{
	LogAddFile(fopen("output.log", "ab+"), LogLevelDebug);

	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	auto window = std::make_shared<CWindow>();
	auto device = std::make_shared<CGPUDevice>(window, argc > 1 ? argv[1] : nullptr);

	Vertex_t vertices[] = {
		{  {0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{ {0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
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

	u64 now = 0;
	u64 last = now;
	while (window->IsOpen())
	{
		now = SDL_GetTicks();
		f32 delta = (now - last) / 1000.0f;

		window->Update();

		world.progress(delta);

		last = now;
	}

	pipeline.reset();
	device.reset();
	window.reset();

	return 0;
}
