#include "game.h"
#include "log.h"
#include "util.h"

#include "gpu/commandbuffer.h"
#include "gpu/renderpass.h"
#include "gpu/shader_locations.h"

#include "mesh.h"
#include "scene.h"

CMesh::CMesh(std::shared_ptr<CGPUDevice> device, SDL_Storage* storage, cstr name)
{
	LogInfo("Loading model %s", name);

	u64 size = 0;
	dstr data = (dstr)Read(storage, name, size);
	if (!data)
	{
		LogError("Failed to read mesh %s: %s", name, SDL_GetError());
		return;
	}

	// this code is all from purpl engine's mesh tool
	// TODO: move this offline like in purpl

	const aiScene* scene = aiImportFileFromMemory(
		data, (u32)size,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_ConvertToLeftHanded, nullptr);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LogError("Failed to parse mesh %s: %s", name, aiGetErrorString());
		return;
	}

	std::vector<const aiMesh*> meshes;
	ProcessNode(scene->mRootNode, scene, meshes);

	std::vector<Vertex> vertices;
	std::vector<Index> indices;
	ConvertMesh(scene, meshes[0], vertices, indices);
}

void CMesh::ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes)
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

void CMesh::ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<Index>& indices)
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