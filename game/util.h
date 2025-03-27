#pragma once

#include "game.h"

extern SDL_Storage* OpenStorage(void);
extern u8 *Read(SDL_Storage* storage, cstr path, u64& size);

extern dstr Format(cstr message, ...);
extern dstr FormatVa(cstr message, va_list args);

extern void Error(cstr message, ...);

extern void EcsLog(s32 Level, cstr File, s32 Line, cstr Message);
extern void *EcsMalloc(ecs_size_t size);
extern void EcsFree(void *block);
extern void *EcsRealloc(void *block, ecs_size_t size);
extern void *EcsCalloc(ecs_size_t size);
