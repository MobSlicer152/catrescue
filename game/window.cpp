#include "window.h"
#include "gpu/device.h"
#include "log.h"
#include "util.h"

CWindow::CWindow(cstr title, u32 width, u32 height) : m_handle(nullptr), m_open(false), m_width(width), m_height(height)
{
	LogInfo("Creating %ux%u window titled %s", m_width, m_height, title);

	m_handle =
		SDL_CreateWindow(title, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
	if (!m_handle)
	{
		LogError("failed to create window: %s", SDL_GetError());
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
			break;
		case SDL_EVENT_QUIT:
			LogInfo("Quit requested");
			m_open = false;
			break;
		}
	}
}
