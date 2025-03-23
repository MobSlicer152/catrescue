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
	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	CWindow* window = new CWindow();

	InitializeGL();
	ResizeGL(window->GetWidth(), window->GetHeight());

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

	CGLVertexBuffer vbo(vertices, ARRAYSIZE(vertices));
	CGLIndexBuffer ebo(indices, ARRAYSIZE(indices));
	CGLVertexArray vao(vbo, ebo);
	CGLShaderProgram shader(storage, "shaders/basic.vert", "shaders/basic.frag");

	glm::mat4 model(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), window->GetAspect(), 0.1f, 1000.0f);
	glm::mat4 camera = projection * view;

	u64 now = 0;
	u64 last = now;
	while (window->IsOpen())
	{
		now = SDL_GetTicks();

		window->Update();

		ClearScreen();

		shader.Bind();
		vao.Bind();
		shader.SetUniform("model", model);
		shader.SetUniform("camera", camera);
		glDrawElements(GL_TRIANGLES, ebo.GetIndexCount() * 3, GL_UNSIGNED_INT, (void*)(0));

		world.progress((now - last) / 1000.0f);

		window->SwapBuffers();

		last = now;
	}

	delete window;

	return 0;
}
