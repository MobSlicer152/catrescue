#include "game/log.h"
#include "game/util.h"
#include "glshader.h"

CGLShaderProgram::CGLShaderProgram(SDL_Storage* storage, cstr vertexName, cstr fragmentName)
{
	LogInfo("Creating shader program from %s and %s", vertexName, fragmentName);

	u32 vertexShader = Load(storage, vertexName, GL_VERTEX_SHADER);
	u32 fragmentShader = Load(storage, fragmentName, GL_FRAGMENT_SHADER);

	m_handle = glCreateProgram();
	if (m_handle == GL_INVALID_VALUE)
	{
		LogError("failed to create shader program: %d!\n", glGetError());
		return;
	}

	glAttachShader(m_handle, vertexShader);
	glAttachShader(m_handle, fragmentShader);
	glLinkProgram(m_handle);

	int32_t success = GL_FALSE;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[512] = {0};
		glGetProgramInfoLog(m_handle, sizeof(errorLog), NULL, errorLog);
		LogError(
			"failed to link shader m_handle from %s and %s: %s\n", vertexName, fragmentName,
			errorLog);
		return;
	}

	glDetachShader(m_handle, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(m_handle, fragmentShader);
	glDeleteShader(fragmentShader);
}

u32 CGLShaderProgram::Load(SDL_Storage* storage, cstr name, GLenum shaderType)
{
	LogInfo("Loading shader %s", name);

	u64 size = 0;
	void* data = Read(storage, name, &size);

	u32 shader = glCreateShader(shaderType);
	cstr sources[] = {(cstr)data};
	s32 sourceSizes[] = {(s32)size};
	glShaderSource(shader, 1, sources, sourceSizes);
	free(data);

	glCompileShader(shader);

	s32 success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[512] = {0};
		glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		glDeleteShader(shader);
		LogError("failed to compile shader %s: %s\n", name, errorLog);
		return GL_INVALID_VALUE;
	}

	return shader;
}