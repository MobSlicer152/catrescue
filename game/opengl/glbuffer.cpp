#include "game/log.h"
#include "glbuffer.h"

void CGLBuffer::Upload(const void* data, ssize size) const
{
    Bind();
    glBufferData(m_type, size, data, m_usage);
    Unbind();
}

CGLVertexBuffer::CGLVertexBuffer(const Vertex_t* vertices, uint32_t vertexCount) : CGLBuffer(GL_STATIC_DRAW, GL_ARRAY_BUFFER)
{
	glGenBuffers(1, &m_handle);
	if (m_handle == GL_INVALID_VALUE)
	{
		LogError("failed to create vertex m_handle: %d!", glGetError());
		return;
	}

	Upload(vertices, vertexCount * sizeof(Vertex_t));
}

CGLIndexBuffer::CGLIndexBuffer(const Index_t* indices, uint32_t indexCount) : CGLBuffer(GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER), m_indexCount(indexCount)
{
	glGenBuffers(1, &m_handle);
	if (m_handle == GL_INVALID_VALUE)
	{
		LogError("failed to create index m_handle: %d!", glGetError());
		return;
	}

	Upload(indices, indexCount * sizeof(Index_t));
}

CGLVertexArray::CGLVertexArray(const CGLVertexBuffer& vertexBuffer, const CGLIndexBuffer& indexBuffer)
{
	glGenVertexArrays(1, &m_handle);
	if (m_handle == GL_INVALID_VALUE)
	{
		LogError("failed to create vertex array: %d!", glGetError());
		return;
	}

	Bind();
	vertexBuffer.Bind();
	indexBuffer.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, texCoord));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, color));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	Unbind();
}
