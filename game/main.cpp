#include "SDL3/SDL_main.h"

#include "game.h"
#include "log.h"
#include "render/buffer.h"
#include "render/device.h"
#include "util.h"
#include "window.h"
#include <memory>

int SDL_main(int argc, char* argv[])
{
	LogAddFile(fopen("output.log", "ab+"), LogLevelDebug);

	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	std::shared_ptr<CWindow> window = std::make_shared<CWindow>();
	std::shared_ptr<CGPUDevice> device = std::make_shared<CGPUDevice>(argc > 1 ? argv[1] : nullptr);
	window->ClaimForDevice(device);

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

	CGPUBuffer vertexBuffer(device, vertices, ARRAYSIZE(vertices));
	CGPUBuffer indexBuffer(device, indices, ARRAYSIZE(indices));

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

	window->ReleaseForDevice(device);

	return 0;
}
