#pragma once

#include "game/game.h"
#include "game/mesh.h"
#include "opengl.h"

class CGLBuffer: public CBaseGLObject
{
  public:
    virtual ~CGLBuffer()
    {
		glDeleteBuffers(1, &m_handle);
    }

	virtual void Bind() const
	{
		glBindBuffer(m_type, m_handle);
	}

	virtual void Unbind() const
	{
		glBindBuffer(m_type, 0);
	}

  protected:
    GLenum m_usage;
    GLenum m_type;

    CGLBuffer(GLenum usage, GLenum type) : m_usage(usage), m_type(type) {}

    void Upload(const void* data, ssize size) const;
};

class CGLVertexBuffer: public CGLBuffer
{
  public:
	CGLVertexBuffer(const Vertex_t* vertices, u32 vertexCount);
};

class CGLIndexBuffer: public CGLBuffer
{
  public:
	CGLIndexBuffer(const Index_t* indices, u32 indexCount);

	u32 GetIndexCount() const
	{
		return m_indexCount;
	}

  private:
	u32 m_indexCount;
};

class CGLVertexArray: public CBaseGLObject
{
  public:
	CGLVertexArray(const CGLVertexBuffer& vertexBuffer, const CGLIndexBuffer& indexBuffer);
	~CGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_handle);
	}

	virtual void Bind() const
	{
		glBindVertexArray(m_handle);
	}

	virtual void Unbind() const
	{
		glBindVertexArray(0);
	}
};