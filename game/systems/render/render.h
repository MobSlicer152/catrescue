#pragma once

#include "components/render.h"
#include "components/transform.h"
#include "game.h"
#include "window.h"

#include "gpu/device.h"
#include "gpu/pipeline.h"
#include "gpu/sampler.h"

#include "mesh.h"
#include "scene.h"
#include "texture.h"
#include "util.h"

struct Material
{
	usize shader;
	usize diffuse;
};

struct Model
{
    usize mesh;
    usize material;
};

class CRenderSystem
{
  public:
    CRenderSystem() = default;
    ~CRenderSystem();

    void Initialize(std::shared_ptr<CWindow> window, cstr api = nullptr);
    void Register(flecs::world& world);

    usize LoadShader(SDL_Storage* storage, cstr name);
    usize LoadTexture(SDL_Storage* storage, cstr name);
    usize LoadMesh(SDL_Storage* storage, cstr name);
    usize CreateMesh(Vertex* vertices, usize vertexCount, Index* indices, usize indexCount);
    usize CreateMaterial(usize shader, usize diffuse);
    usize CreateModel(usize mesh, usize material);

    void DrawModel(usize model, const ObjectUBO& objectUbo, const SceneUBO& sceneUbo);

    CGPUGraphicsPipeline* GetShader(usize id)
    {
        return GetObject(id, m_shaders);
    }

    CTexture* GetTexture(usize id)
    {
        return GetObject(id, m_textures);
    }

    CMesh* GetMesh(usize id)
    {
        return GetObject(id, m_meshes);
    }

    Material* GetMaterial(usize id)
    {
        return GetObject(id, m_materials);
    }

    Model* GetModel(usize id)
    {
        return GetObject(id, m_models);
    }

  private:
    std::shared_ptr<CWindow> m_window;
    std::shared_ptr<CGPUDevice> m_device;
    std::shared_ptr<CGPUSampler> m_sampler;
    std::shared_ptr<CGPUCommandBuffer> m_cmdBuf;
    std::shared_ptr<CGPURenderPass> m_renderPass;

    std::vector<CGPUGraphicsPipeline> m_shaders;
    std::vector<CTexture> m_textures;
    std::vector<CMesh> m_meshes;
    std::vector<Material> m_materials;
    std::vector<Model> m_models;

    flecs::system m_frameStartSystem;
    flecs::system m_drawSystem;
    flecs::system m_frameEndSystem;

    void Begin();
    void DrawEntity(flecs::entity entity, EntityModel& model, EntityPosition& position);
    void End();
};

extern CRenderSystem RenderSystem;
