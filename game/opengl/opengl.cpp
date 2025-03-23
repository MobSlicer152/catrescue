#include "game/game.h"
#include "game/log.h"
#include "game/util.h"
#include "opengl.h"

void InitializeGL()
{
	LogInfo("Loading OpenGL");

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		Error("Failed to load OpenGL");
	}

	LogInfo("Got OpenGL %s context", glGetString(GL_VERSION));

	//glEnable(GL_DEPTH);
	//glEnable(GL_DEPTH_TEST);
}

void ClearScreen(glm::vec4 color, f32 depth)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClearDepthf(depth);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ResizeGL(u32 width, u32 height)
{
	glScissor(0, 0, width, height);
	glViewport(0, 0, width, height);
}

