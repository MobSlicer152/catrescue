#pragma once

#include "game.h"
#include "mesh.h"

#include "gpu/buffer.h"

#include "material.h"
#include "scene.h"

class CModel
{
  public:
	CModel(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr name, std::shared_ptr<CMaterial> material);
	CModel(
		std::shared_ptr<CGPUDevice> device, const Vertex* vertices, u32 vertexCount, const Index* indices, u32 indexCount,
		std::shared_ptr<CMaterial> material)
		: m_vertexBuffer(std::make_shared<CGPUBuffer>(device, vertices, vertexCount)),
		  m_indexBuffer(std::make_shared<CGPUBuffer>(device, indices, indexCount)), m_material(material)
	{
	}
	~CModel() = default;

	void Draw(CGPURenderPass& renderPass, const ObjectUBO& objectUbo, const SceneUBO& sceneUbo);

	std::shared_ptr<CMaterial> GetMaterial() const
	{
		return m_material;
	}

  private:
	std::shared_ptr<CGPUBuffer> m_vertexBuffer;
	std::shared_ptr<CGPUBuffer> m_indexBuffer;
	std::shared_ptr<CMaterial> m_material;

	// recursively flattens the node graph
	static void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes);
	void ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<Index>& indices);
};