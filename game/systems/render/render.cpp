#include "game.h"
#include "window.h"

#include "components/render.h"
#include "components/transform.h"

#include "gpu/commandbuffer.h"
#include "gpu/device.h"
#include "gpu/pipeline.h"
#include "gpu/shader.h"
#include "gpu/shader_locations.h"
#include "gpu/texture.h"

#include "render.h"
#include "texture.h"

CRenderSystem RenderSystem;

void CRenderSystem::Initialize(std::shared_ptr<CWindow> window, cstr api)
{
	m_window = window;
	m_device = std::make_shared<CGPUDevice>(m_window, api);

	SDL_GPUSamplerCreateInfo samplerInfo = {};
	samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
	samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
	samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
	samplerInfo.min_lod = 0.0f;
	samplerInfo.max_lod = 1.0f;
	samplerInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
	samplerInfo.min_filter = SDL_GPU_FILTER_NEAREST;
	m_sampler = std::make_shared<CGPUSampler>(m_device, samplerInfo);
}

CRenderSystem::~CRenderSystem()
{
}

void CRenderSystem::Register(flecs::world& world)
{
	m_frameStartSystem = world.system<>("RenderBegin").kind(flecs::PreFrame).run([this](flecs::iter& iter) { Begin(); });
	m_drawSystem =
		world.system<EntityModel, EntityPosition>("RenderDrawObjects")
			.kind(flecs::PostUpdate)
			.each([this](flecs::entity e, EntityModel& model, EntityPosition& position) { DrawEntity(e, model, position); });
	m_frameEndSystem = world.system<>("RenderEnd").kind(flecs::PostFrame).run([this](flecs::iter& iter) { End(); });
}

usize CRenderSystem::LoadShader(SDL_Storage* storage, cstr name)
{
	auto vertexShader = std::make_shared<CGPUShader>(m_device, storage, name, SDL_GPU_SHADERSTAGE_VERTEX);
	auto fragmentShader = std::make_shared<CGPUShader>(m_device, storage, name, SDL_GPU_SHADERSTAGE_FRAGMENT);

	SDL_GPUColorTargetDescription colorTarget = {
		m_device->GetSwapChainFormat(),
		{SDL_GPU_BLENDFACTOR_SRC_COLOR, SDL_GPU_BLENDFACTOR_DST_COLOR, SDL_GPU_BLENDOP_ADD, SDL_GPU_BLENDFACTOR_SRC_ALPHA,
								SDL_GPU_BLENDFACTOR_DST_ALPHA, SDL_GPU_BLENDOP_ADD,
								SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G | SDL_GPU_COLORCOMPONENT_B | SDL_GPU_COLORCOMPONENT_A, true, true},
	};

	GPUGraphicsPipelineCreateInfo_t pipelineInfo = {
		*vertexShader,
		*fragmentShader,
		StandardVertex,
		SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		SDL_GPU_SAMPLECOUNT_1,
		{
          SDL_GPU_FILLMODE_FILL, SDL_GPU_CULLMODE_BACK,
          SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE, },
		{
          SDL_GPU_COMPAREOP_LESS, {SDL_GPU_STENCILOP_KEEP, SDL_GPU_STENCILOP_KEEP},
          {SDL_GPU_STENCILOP_KEEP, SDL_GPU_STENCILOP_KEEP},
          0x00, 0x00,
          true, true,
          false, },
		&colorTarget,
		1,
		SDL_GPU_TEXTUREFORMAT_INVALID
    };
	m_shaders.emplace_back(m_device, pipelineInfo);
	vertexShader.reset();
	fragmentShader.reset();

	return m_shaders.size() - 1;
}

usize CRenderSystem::LoadTexture(SDL_Storage* storage, cstr name)
{
	auto texture = std::make_shared<CGPUTexture>(m_device, storage, name, SDL_GPU_TEXTUREUSAGE_SAMPLER);
	m_textures.emplace_back(m_sampler, texture);
	return m_textures.size() - 1;
}

usize CRenderSystem::LoadMesh(SDL_Storage* storage, cstr name)
{
	m_meshes.emplace_back(m_device, storage, name);
	return m_meshes.size() - 1;
}

usize CRenderSystem::CreateMesh(Vertex* vertices, usize vertexCount, Index* indices, usize indexCount)
{
	m_meshes.emplace_back(m_device, vertices, vertexCount, indices, indexCount);
	return m_meshes.size() - 1;
}

usize CRenderSystem::CreateMaterial(usize shader, usize diffuse)
{
	m_materials.emplace_back(shader, diffuse);
	return m_materials.size() - 1;
}

usize CRenderSystem::CreateModel(usize mesh, usize material)
{
	m_models.emplace_back(mesh, material);
	return m_models.size() - 1;
}

void CRenderSystem::DrawModel(usize model, const ObjectUBO& objectUbo, const SceneUBO& sceneUbo)
{
	auto modelRef = GetModel(model);
	auto mesh = GetMesh(modelRef->mesh);
	auto material = GetMaterial(modelRef->material);
	auto shader = GetShader(material->shader);
	auto diffuse = GetTexture(material->diffuse);

	if (!modelRef || !mesh || !material || !shader || !diffuse)
	{
		return;
	}

	m_renderPass->BindVertexBuffer(mesh->m_vertexBuffer);
	m_renderPass->BindIndexBuffer(mesh->m_indexBuffer);

	m_cmdBuf->PushVertexUniform(SCENE_UBO_LOCATION, &sceneUbo, sizeof(SceneUBO));
	m_cmdBuf->PushVertexUniform(OBJECT_UBO_LOCATION, &objectUbo, sizeof(ObjectUBO));

	m_renderPass->BindGraphicsPipeline(*shader);
	diffuse->Bind(*m_renderPass);

	m_renderPass->DrawIndexed(mesh->m_indexBuffer->GetIndexCount());
}

void CRenderSystem::Begin()
{
	m_cmdBuf = std::make_shared<CGPUCommandBuffer>(m_device);
	m_renderPass = std::make_shared<CGPURenderPass>(
		m_cmdBuf, m_cmdBuf->GetSwapChainTexture(m_window), nullptr, glm::vec4(0.2, 0.2, 0.2, 1.0));
}

void CRenderSystem::DrawEntity(flecs::entity entity, EntityModel& model, EntityPosition& position)
{
	constexpr EntityRotation DEFAULT_ROTATION = {glm::identity<glm::quat>()};
	constexpr EntityScale DEFAULT_SCALE = {glm::vec3(1.0, 1.0, 1.0)};

	auto rotation = entity.get<EntityRotation>();
	auto scale = entity.get<EntityScale>();

	rotation = rotation ? rotation : &DEFAULT_ROTATION;
	scale = scale ? scale : &DEFAULT_SCALE;
	auto transform = CalculateTransform(position, *rotation, *scale);
	DrawModel(model.id, ObjectUBO{transform}, SceneUBO{});
}

void CRenderSystem::End()
{
	m_renderPass->End();
	m_cmdBuf->Submit();

	m_renderPass.reset();
	m_cmdBuf.reset();
}
