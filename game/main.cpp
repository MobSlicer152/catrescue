#include "SDL3/SDL_main.h"

#include "game.h"
#include "log.h"
#include "render/opengl/opengl.h"
#include "util.h"
#include "window.h"

int SDL_main(int argc, char* argv[])
{
	LogAddFile(fopen("output.log", "ab+"), LogLevelDebug);

	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	CWindow* window = new CWindow();

	InitializeGL();
	ResizeGL(window->GetWidth(), window->GetHeight());

	u64 now = 0;
	u64 last = now;
	while (window->IsOpen())
	{
		now = SDL_GetTicks();
		f32 delta = (now - last) / 1000.0f;

		window->Update();

		ClearScreen();
		world.progress(delta);
		window->SwapBuffers();

		last = now;
	}

	delete window;

	return 0;
}
