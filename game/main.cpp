#include "SDL3/SDL_main.h"

#include "game.h"
#include "log.h"
#include "model.h"
#include "opengl/glbuffer.h"
#include "opengl/glshader.h"
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

	CGLShaderProgram shader(storage, "shaders/basic.vert", "shaders/basic.frag");
	CGLTexture texture(storage, "textures/missing.qoi");
	CGLMaterial material(&shader, &texture);

	CModel teapot(storage, "models/teapot.obj", &material);

	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), window->GetAspect(), 0.1f, 1000.0f);

	u64 now = 0;
	u64 last = now;
	while (window->IsOpen())
	{
		now = SDL_GetTicks();
		f32 delta = (now - last) / 1000.0f;

		window->Update();

		ClearScreen();

		model = glm::rotate(model, glm::radians(10.0f * delta), glm::vec3(0.0f, 1.0f, 0.0f));
		teapot.Draw(model, view, proj);

		world.progress(delta);

		window->SwapBuffers();

		last = now;
	}

	delete window;

	return 0;
}
