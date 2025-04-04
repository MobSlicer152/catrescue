#define QOI_IMPLEMENTATION
#define QOI_NO_STDIO

#include "game.h"
#include "log.h"
#include "texture.h"
#include "util.h"

u32* LoadImage(SDL_Storage* storage, cstr name, u32& width, u32& height, SDL_GPUTextureFormat& format)
{
    u64 size = 0;
    void* raw = Read(storage, name, size);
    if (!raw)
    {
        LogError("Failed to read image %s: %s", name, SDL_GetError());
        return nullptr;
    }

    qoi_desc desc = {};
    u32* data = (u32*)qoi_decode(raw, (s32)size, &desc, 4);
    if (!data || desc.channels != 4)
    {
        LogError("Failed to decode image %s");
        free(raw);
        return nullptr;
    }

    switch (desc.colorspace)
    {
    case QOI_LINEAR:
        format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        break;
    case QOI_SRGB:
        format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM_SRGB;
        break;
    }

    free(raw);

    width = desc.width;
    height = desc.height;
    return data;
}
