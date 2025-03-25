#pragma once

#include "game/game.h"
#include "game/render/texture.h"
#include "opengl.h"

class CGLTexture: public CBaseGLObject, public ITexture
{
  public:
	CGLTexture() : CBaseGLObject() {}
	CGLTexture(const CGLTexture& other) : CBaseGLObject(other), m_width(other.m_width), m_height(other.m_height) {}

	CGLTexture(SDL_Storage* storage, cstr name);
	~CGLTexture()
	{
		if (IsGood())
		{
			glDeleteTextures(1, &m_handle);
            m_handle = GL_INVALID_VALUE;
		}
	}

	virtual void Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_handle);
	}

	virtual u32 GetWidth() const
	{
		return m_width;
	}

	virtual u32 GetHeight() const
	{
		return m_height;
	}

  private:
	u32 m_width;
	u32 m_height;
};
