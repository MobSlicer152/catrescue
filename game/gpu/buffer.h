#pragma once

#include "game/game.h"
#include "game/mesh.h"
#include "gpuobj.h"

class CGPUDevice;

class CGPUBuffer: public CBaseGPUObject<SDL_GPUBuffer>
{
  public:
	CGPUBuffer(std::shared_ptr<CGPUDevice> device, SDL_GPUBufferUsageFlags usage, const void* data, u32 size);

	CGPUBuffer(std::shared_ptr<CGPUDevice> device, const Vertex* vertices, u32 vertexCount)
		: CGPUBuffer(
			  device, SDL_GPU_BUFFERUSAGE_VERTEX | SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, vertices,
			  vertexCount * sizeof(Vertex))
	{
	}

	CGPUBuffer(std::shared_ptr<CGPUDevice> device, const Index* indices, u32 indexCount)
		: CGPUBuffer(
			  device, SDL_GPU_BUFFERUSAGE_INDEX | SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, indices,
			  indexCount * sizeof(Index))
	{
	}

	~CGPUBuffer();

	bool IsVertexBuffer() const
	{
		return m_usage & SDL_GPU_BUFFERUSAGE_VERTEX;
	}

	bool IsIndexBuffer() const
	{
		return m_usage & SDL_GPU_BUFFERUSAGE_VERTEX;
	}

	u32 GetSize() const
	{
		return m_size;
	}

	u32 GetVertexCount() const
	{
		return m_size / sizeof(Vertex);
	}

	u32 GetIndexCount() const
	{
		return m_size / sizeof(Index) * 3;
	}

	SDL_GPUBufferUsageFlags GetUsage() const
	{
		return m_usage;
	}

  private:
	SDL_GPUBufferUsageFlags m_usage;
	u32 m_size;
};
