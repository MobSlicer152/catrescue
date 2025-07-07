#include "game.h"
#include "mesh.h"

#include "gpu/commandbuffer.h"
#include "gpu/renderpass.h"
#include "gpu/shader_locations.h"

#include "material.h"
#include "scene.h"
#include "model.h"

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

void CModel::ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<Index>& indices)
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

void CModel::Draw(CGPURenderPass& renderPass, const ObjectUBO& objectUbo, const SceneUBO& sceneUbo)
{
    auto cmdBuf = renderPass.GetParent();

	renderPass.BindVertexBuffer(m_vertexBuffer);
	renderPass.BindIndexBuffer(m_indexBuffer);

	cmdBuf->PushVertexUniform(SCENE_UBO_LOCATION, &sceneUbo, sizeof(SceneUBO));
	cmdBuf->PushVertexUniform(OBJECT_UBO_LOCATION, &objectUbo, sizeof(ObjectUBO));
	m_material->Bind(renderPass);

	renderPass.DrawIndexed(m_indexBuffer->GetIndexCount());
}