#include "window.h"
#include "log.h"
#include "opengl/opengl.h"
#include "util.h"

CWindow::CWindow(cstr title, u32 width, u32 height) : m_handle(nullptr), m_open(false), m_width(width), m_height(height)
{
	LogInfo("Creating %ux%u window titled %s", m_width, m_height, title);

	// 4.2 is the highest that Gallium or whatever supports
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#ifdef GAME_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	m_handle =
		SDL_CreateWindow(title, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
	if (!m_handle)
	{
		LogError("failed to create window: %s", SDL_GetError());
		return;
	}

	LogInfo("Creating OpenGL context");

	m_context = SDL_GL_CreateContext(m_handle);
	if (!m_context)
	{
		LogError("Failed to create OpenGL context: %s", SDL_GetError());
		return;
	}

	m_open = true;
}

CWindow::~CWindow()
{
	if (m_handle)
	{
		LogInfo("Destroying window %s", SDL_GetWindowTitle(m_handle));
		SDL_DestroyWindow(m_handle);
		m_handle = nullptr;
	}
}

void CWindow::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_WINDOW_RESIZED:
			LogInfo("Window resized from %ux%u to %ux%u", m_width, m_height, event.window.data1, event.window.data2);
			m_width = event.window.data1;
			m_height = event.window.data2;
			ResizeGL(m_width, m_height);
			break;
		case SDL_EVENT_QUIT:
			LogInfo("Quit requested");
			m_open = false;
			break;
		}
	}
}

void CWindow::SwapBuffers()
{
	SDL_GL_SwapWindow(m_handle);
}
