#include "game/log.h"
#include "game/texture.h"
#include "game/util.h"
#include "gltexture.h"

CGLTexture::CGLTexture(SDL_Storage* storage, cstr name)
{
    u32* data = LoadImage(storage, name, m_width, m_height);
    if (!data)
    {
        return;
    }

    glGenTextures(1, &m_handle);
    if (m_handle == GL_INVALID_VALUE)
    {
        LogError("Failed to create GL texture: %s", glGetError());
        return;
    }

    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(data);
}
