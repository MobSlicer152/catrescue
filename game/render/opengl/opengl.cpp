#include "game/game.h"
#include "game/log.h"
#include "game/util.h"
#include "opengl.h"

static void APIENTRY GLDebugLog(GLenum source, GLenum type, u32 id, GLenum severity, s32 length, cstr msg, const void* user)
{
	LogDebug("GL message: source 0x%X type 0x%X id 0x%X: %s", source, type, id, msg);
}

void InitializeGL()
{
	LogInfo("Loading OpenGL");

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		Error("Failed to load OpenGL");
	}

	LogInfo("Got OpenGL %s context", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

#ifdef GAME_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallback(GLDebugLog, nullptr);
#endif
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

