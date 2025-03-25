#pragma once

#include "game/game.h"

extern void InitializeGL();
extern void ClearScreen(glm::vec4 color = {0.0f, 0.0f, 0.0f, 0.0f}, f32 depth = 1.0f);
extern void ResizeGL(u32 width, u32 height);

class CBaseGLObject
{
  public:
	CBaseGLObject() : m_handle(GL_INVALID_VALUE) {}
	CBaseGLObject(const CBaseGLObject& other) : m_handle(other.m_handle) {}

	virtual ~CBaseGLObject() = default;

	virtual void Bind() const = 0;

	virtual u32 GetHandle() const
	{
		return m_handle;
	}

	virtual bool IsGood() const
	{
		return m_handle != GL_INVALID_VALUE;
	}

  protected:
	u32 m_handle;
};
