#pragma once

#include "game.h"
#include "mesh.h"

#include "gpu/buffer.h"

class CMesh
{
  public:
	CMesh(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr name);
	CMesh(
		std::shared_ptr<CGPUDevice> device, const Vertex* vertices, u32 vertexCount, const Index* indices, u32 indexCount)
		: m_vertexBuffer(std::make_shared<CGPUBuffer>(device, vertices, vertexCount)),
		  m_indexBuffer(std::make_shared<CGPUBuffer>(device, indices, indexCount))
	{
	}
	~CMesh() = default;

  private:
    friend class CRenderSystem;

	std::shared_ptr<CGPUBuffer> m_vertexBuffer;
	std::shared_ptr<CGPUBuffer> m_indexBuffer;

	// recursively flattens the node graph
	static void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes);
	void ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<Index>& indices);
};