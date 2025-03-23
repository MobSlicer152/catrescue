#pragma once

#include "game.h"
#include "opengl/glbuffer.h"
#include "opengl/glmaterial.h"

class CModel
{
  public:
	CModel(SDL_Storage* storage, cstr name, void* material);
	~CModel() = default;

	void Draw();
};
