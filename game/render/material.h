#pragma once

#include "game.h"

#include "gpu/pipeline.h"
#include "gpu/renderpass.h"
#include "gpu/texture.h"

#include "material.h"
#include "texture.h"

class CMaterial
{
  public:
	CMaterial(std::shared_ptr<CGPUGraphicsPipeline> shader, std::shared_ptr<CTexture> diffuse) : m_shader(shader), m_diffuse(diffuse)
	{
	}
	~CMaterial() = default;

    void Bind(CGPURenderPass& renderPass) const
    {
        m_diffuse->Bind(renderPass);
        renderPass.BindGraphicsPipeline(m_shader);
    }

	std::shared_ptr<CGPUGraphicsPipeline> GetShader() const
	{
		return m_shader;
	}

	std::shared_ptr<CTexture> GetDiffuse() const
	{
		return m_diffuse;
	}

  private:
	std::shared_ptr<CGPUGraphicsPipeline> m_shader;
	std::shared_ptr<CTexture> m_diffuse;
};