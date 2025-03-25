#pragma once

#include "game/game.h"
#include "game/render/model.h"
#include "glbuffer.h"
#include "glmaterial.h"

class CGLModel: public IModel
{
  public:
	CGLModel(SDL_Storage* storage, cstr name, const CGLMaterial* material);
	~CGLModel() = default;

	virtual void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj);

	virtual const IMaterial* GetMaterial() const
	{
		return m_material;
	}

  private:
	bool m_good;
	CGLVertexBuffer m_vbo;
	CGLIndexBuffer m_ebo;
	CGLVertexArray m_vao;
	const CGLMaterial* m_material;

	// recursively flattens the node graph
	static void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes);
	void ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex_t>& vertices, std::vector<Index_t>& indices);

	CGLModel(const Vertex_t* vertices, u32 vertexCount, const Index_t* indices, u32 indexCount) : m_vbo(vertices, vertexCount), m_ebo(indices, indexCount), m_vao(m_vbo, m_ebo)
	{
	}
};
