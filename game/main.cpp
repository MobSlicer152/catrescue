#include "SDL3/SDL_main.h"

#include "components/render.h"
#include "components/transform.h"
#include "game.h"
#include "gpu/commandbuffer.h"
#include "gpu/device.h"
#include "gpu/renderpass.h"
#include "gpu/shader_locations.h"
#include "log.h"
#include "systems/render/render.h"
#include "util.h"
#include "window.h"

int main(int argc, char* argv[])
{
	LogAddFile(fopen("output.log", "ab+"), LogLevelDebug);

	SDL_Storage* storage = OpenStorage();

	flecs::world world(argc, argv);

	auto window = std::make_shared<CWindow>();

	RenderSystem.Initialize(window, argc > 1 ? argv[1] : nullptr);
    RenderSystem.Register(world);

	Vertex vertices[] = {
		{  {0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{ {0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
	};

	Index indices[] = {
		{0, 1, 2},
        {0, 2, 3}
    };

    auto shader = RenderSystem.LoadShader(storage, "main");
	auto texture = RenderSystem.LoadTexture(storage, "textures/missing.qoi");
	auto mesh = RenderSystem.CreateMesh(vertices, ARRAYSIZE(vertices), indices, ARRAYSIZE(indices));
    auto material = RenderSystem.CreateMaterial(shader, texture);
    auto model = RenderSystem.CreateModel(mesh, material);

    auto entity = world.entity("test");
    entity.set(EntityPosition { glm::vec3(0.0, 0.0, 0.0) });
    entity.set(EntityModel { model });

	SceneUBO sceneUbo = {};
	sceneUbo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	sceneUbo.projection = glm::perspective(glm::radians(90.0f), window->GetAspect(), 0.1f, 1000.0f);

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

		world.progress(delta);

		last = now;
	}

	return 0;
}
