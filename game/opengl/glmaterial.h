#pragma once

#include "game/game.h"
#include "glshader.h"
#include "gltexture.h"
#include "opengl.h"

class CGLMaterial
{
  public:
	CGLMaterial(const CGLShaderProgram* shader, const CGLTexture* diffuse) : m_shader(shader), m_diffuse(diffuse)
	{
	}
	~CGLMaterial() = default;

	void Bind() const
	{
		GetShader().Bind();
        glActiveTexture(GL_TEXTURE0);
		GetDiffuse().Bind();
	}

    const CGLShaderProgram& GetShader() const
    {
        return *m_shader;
    }

    const CGLTexture& GetDiffuse() const
    {
        return *m_diffuse;
    }

  private:
	const CGLShaderProgram* m_shader;
	const CGLTexture* m_diffuse;
};
