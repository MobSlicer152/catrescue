#pragma once

#include "game/game.h"
#include "game/log.h"
#include "game/render/shader.h"
#include "opengl.h"

class CGLShaderProgram: public CBaseGLObject, public IShaderProgram
{
  public:
	CGLShaderProgram(SDL_Storage* storage, cstr vertexName, cstr fragmentName);
    CGLShaderProgram(const CGLShaderProgram& other) : CBaseGLObject(other) {}

	~CGLShaderProgram()
	{
        if (IsGood())
        {
		    glDeleteProgram(m_handle);
            m_handle = GL_INVALID_VALUE;
        }
	}

	virtual void Bind() const
	{
		glUseProgram(m_handle);
	}

	virtual s32 GetUniformLocation(cstr name) const
	{
		return glGetUniformLocation(m_handle, name);
	}

	virtual void SetUniform(cstr name, glm::vec2 value) const
	{
		glUniform2fv(GetUniformLocation(name), 1, (f32*)&value);
	}

	virtual void SetUniform(cstr name, glm::vec3 value) const
	{
		glUniform3fv(GetUniformLocation(name), 1, (f32*)&value);
	}

	virtual void SetUniform(cstr name, glm::vec4 value) const
	{
		glUniform4fv(GetUniformLocation(name), 1, (f32*)&value);
	}

	virtual void SetUniform(cstr name, glm::mat4 value) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, false, (f32*)&value);
	}

  private:
	static u32 Load(SDL_Storage* storage, cstr name, GLenum shaderType);
};
