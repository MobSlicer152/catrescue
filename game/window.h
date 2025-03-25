#pragma once

#include "game.h"
#include "render/device.h"


class CWindow
{
  public:
    CWindow(cstr title = GAME_NAME, u32 width = 1024, u32 height = 576);
    ~CWindow();

    void Update();
	void SwapBuffers();

    bool IsOpen() const
    {
        return m_open;
	}

	bool ClaimForDevice(const CRenderDevice& device) const;

	u32 GetWidth() const
	{
		return m_width;
	}

	u32 GetHeight() const
	{
		return m_height;
	}

	f32 GetAspect() const
	{
		return (f32)m_width / (f32)m_height;
	}

  private:
    SDL_Window* m_handle;
	SDL_GLContext m_context;
    bool m_open;
    u32 m_width;
    u32 m_height;
};
