#pragma once

#include "game/game.h"
#include "game/mesh.h"

class CGPUDevice;

class CGPUBuffer
{
  public:
	CGPUBuffer(const CGPUDevice* device, SDL_GPUBufferUsageFlags usage, const void* data, u32 size);

	CGPUBuffer(const CGPUDevice* device, const Vertex_t* vertices, u32 vertexCount)
		: CGPUBuffer(
			  device, SDL_GPU_BUFFERUSAGE_VERTEX | SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, vertices,
			  vertexCount * sizeof(Vertex_t))
	{
	}

	CGPUBuffer(const CGPUDevice* device, const Index_t* indices, u32 indexCount)
		: CGPUBuffer(
			  device, SDL_GPU_BUFFERUSAGE_INDEX | SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, indices,
			  indexCount * sizeof(Index_t))
	{
	}

	~CGPUBuffer();

	SDL_GPUBuffer* GetHandle() const
	{
		return m_handle;
	}

	bool IsGood() const
	{
		return m_handle != nullptr;
	}

	bool IsVertexBuffer() const
	{
		return m_usage & SDL_GPU_BUFFERUSAGE_VERTEX;
	}

	bool IsIndexBuffer() const
	{
		return m_usage & SDL_GPU_BUFFERUSAGE_VERTEX;
	}

	SDL_GPUBufferUsageFlags GetUsage() const
	{
		return m_usage;
	}

  protected:
	const CGPUDevice* m_device;
	SDL_GPUBuffer* m_handle;
	SDL_GPUBufferUsageFlags m_usage;
};
