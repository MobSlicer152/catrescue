#pragma once

#include "game/game.h"
#include "opengl.h"

class CGLTexture: public CBaseGLObject
{
  public:
	CGLTexture(SDL_Storage* storage, cstr name);
};