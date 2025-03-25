#pragma once

#include "game/game.h"
#include "game/render/material.h"
#include "glshader.h"
#include "gltexture.h"
#include "opengl.h"

class CGLMaterial: public IMaterial
{
  public:
	~CGLMaterial() = default;

	void Bind() const
	{
		m_shader->Bind();
		glActiveTexture(GL_TEXTURE0);
		m_diffuse->Bind();
	}

	const IShaderProgram* GetShader() const
	{
		return m_shader;
	}

	const ITexture* GetDiffuse() const
	{
		return m_diffuse;
	}

  private:
	const CGLShaderProgram* m_shader;
	const CGLTexture* m_diffuse;
	
	CGLMaterial(const CGLShaderProgram* shader, const CGLTexture* diffuse) : m_shader(shader), m_diffuse(diffuse)
	{
	}
};
