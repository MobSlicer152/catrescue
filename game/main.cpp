#include "SDL3/SDL_main.h"

#include "game.h"
#include "log.h"
#include "render/buffer.h"
#include "render/device.h"
#include "util.h"
#include "window.h"

int SDL_main(int argc, char* argv[])
{
	LogAddFile(fopen("output.log", "ab+"), LogLevelDebug);

	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	CWindow* window = new CWindow();
	CGPUDevice* device = new CGPUDevice(argc > 1 ? argv[1] : nullptr);
	window->ClaimForDevice(device);

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
	delete device;
	delete window;

	return 0;
}
