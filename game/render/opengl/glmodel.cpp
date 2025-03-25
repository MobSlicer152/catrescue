#include "game/mesh.h"
#include "game/log.h"
#include "game/util.h"
#include "glmodel.h"

void CGLModel::ProcessNode(const aiNode* node, const aiScene* scene, std::vector<const aiMesh*>& meshes)
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

void CGLModel::ConvertMesh(const aiScene* scene, const aiMesh* mesh, std::vector<Vertex_t>& vertices, std::vector<Index_t>& indices)
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

void CGLModel::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    m_material->Bind();
	m_material->GetShader()->SetUniform("model", model);
	m_material->GetShader()->SetUniform("view", view);
	m_material->GetShader()->SetUniform("proj", proj);
	m_vao.Bind();
	glDrawElements(GL_TRIANGLES, m_ebo.GetIndexCount() * 3, GL_UNSIGNED_INT, (void*)(0));
    m_vao.Unbind();
}
