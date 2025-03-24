#include "mesh.h"
#include "model.h"
#include "log.h"
#include "util.h"

CModel::CModel(SDL_Storage* storage, cstr meshName, const CGLMaterial* material) : m_good(false), m_material(material)
{
	LogInfo("Loading model %s", meshName);

	u64 size = 0;
	dstr data = (dstr)Read(storage, meshName, size);
	if (!data)
	{
		LogError("Failed to read mesh %s: %s", meshName, SDL_GetError());
		return;
	}

    // this code is all from purpl engine's mesh tool
	// TODO: move this offline like in purpl

	const aiScene* scene = aiImportFileFromMemory(
		data, (u32)size, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_ConvertToLeftHanded,
		nullptr);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LogError("Failed to parse mesh %s: %s", meshName, aiGetErrorString());
        return;
    }

    std::vector<const aiMesh*> meshes;
    ProcessNode(scene->mRootNode, scene, meshes);

    std::vector<Vertex_t> vertices;
    std::vector<Index_t> indices;
    ConvertMesh(scene, meshes[0], vertices, indices);

    m_vbo = new CGLVertexBuffer(vertices.data(), (u32)vertices.size());
    m_ebo = new CGLIndexBuffer(indices.data(), (u32)indices.size());
    m_vao = new CGLVertexArray(*m_vbo, *m_ebo);

	m_good = true;
}

void CModel::ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes)
{
    std::vector<u32> nodeMeshes(node->mMeshes, node->mMeshes + node->mNumMeshes);
    meshes.reserve(meshes.capacity() + nodeMeshes.size());
    for (u32 mesh : nodeMeshes)
    {
        meshes.push_back(scene->mMeshes[mesh]);
    }

    for (u32 i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, meshes);
    }
}

void CModel::ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex_t>& vertices, std::vector<Index_t>& indices)
{
    vertices.resize(mesh->mNumVertices);
    for (u32 i = 0; i < mesh->mNumVertices; i++)
    {
        vertices[i].position.x = mesh->mVertices[i].x;
        vertices[i].position.y = mesh->mVertices[i].y;
        vertices[i].position.z = mesh->mVertices[i].z;

        vertices[i].color = glm::vec4(1.0f);

        if (mesh->mNormals)
        {
            vertices[i].normal.x = mesh->mNormals[i].x;
            vertices[i].normal.y = mesh->mNormals[i].y;
            vertices[i].normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0])
        {
            vertices[i].texCoord.s = mesh->mTextureCoords[0][i].x;
            vertices[i].texCoord.t = mesh->mTextureCoords[0][i].y;
        }
    }

    indices.resize(mesh->mNumFaces);
    for (u32 i = 0; i < mesh->mNumFaces; i++)
    {
        indices[i][0] = mesh->mFaces[i].mIndices[0];
        indices[i][1] = mesh->mFaces[i].mIndices[1];
        indices[i][2] = mesh->mFaces[i].mIndices[2];
    }
}

void CModel::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    GetMaterial().Bind();
	GetMaterial().GetShader().SetUniform("model", model);
	GetMaterial().GetShader().SetUniform("view", view);
	GetMaterial().GetShader().SetUniform("proj", proj);
	m_vao->Bind();
	glDrawElements(GL_TRIANGLES, m_ebo->GetIndexCount() * 3, GL_UNSIGNED_INT, (void*)(0));
    m_vao->Unbind();
}
