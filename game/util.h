#pragma once

#include "game.h"

// my brain can't simplify this macro right now, but it surely is possible
#define WRAPPED_ADD(value, add, min, max) ((value) = CLAMP(((value) + 1) % (max), (min), (max)))
#define WRAPPED_INCREMENT(value, min, max) WRAPPED_ADD((value), 1, (min), (max))

// winnt.h defines this as the same thing but can't leave that up to chance
#undef ARRAYSIZE
#define ARRAYSIZE(array) (sizeof(array) / sizeof(array[0]))

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
