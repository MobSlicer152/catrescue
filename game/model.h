#pragma once

#include "game.h"
#include "opengl/glbuffer.h"
#include "opengl/glmaterial.h"

class CModel
{
  public:
	CModel(SDL_Storage* storage, cstr name, const CGLMaterial* material);
	~CModel()
	{
		delete m_vao;
		delete m_ebo;
		delete m_vbo;
	}

	void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj);

	const CGLMaterial& GetMaterial() const
	{
		return *m_material;
	}

	bool IsGood() const
	{
		return m_good;
	}

  private:
	bool m_good;
	CGLVertexBuffer* m_vbo;
	CGLIndexBuffer* m_ebo;
	CGLVertexArray* m_vao;
	const CGLMaterial* m_material;

	// recursively flattens the node graph
	static void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes);
	void ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex_t>& vertices, std::vector<Index_t>& indices);
};
