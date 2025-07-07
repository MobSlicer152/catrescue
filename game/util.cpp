#include "log.h"
#include "util.h"

SDL_Storage* OpenStorage(void)
{
    cstr path = "assets";

    LogInfo("Opening storage in %s", path);
    SDL_Storage* storage = SDL_OpenFileStorage(path);
    if (!storage)
    {
        Error("Failed to open storage %s: %s", path, SDL_GetError());
    }

    while (!SDL_StorageReady(storage))
    {
        LogInfo("Waiting 1 second for storage");
        SDL_Delay(1000);
    }

    return storage;
}

u8 *Read(SDL_Storage* storage, cstr path, u64& size)
{
    if (!SDL_GetStorageFileSize(storage, path, &size) || size < 1)
    {
        Error("failed to read file %s or it was empty: %s", path, SDL_GetError());
    }

    u8 *data = new u8[size];
    if (!data)
    {
        Error("failed to allocate memory for file %s", path);
    }

    if (!SDL_ReadStorageFile(storage, path, data, size))
    {
        Error("failed to read file %s: %s", path, SDL_GetError());
    }

    return data;
}

dstr Format(cstr message, ...)
{
    va_list args;

    va_start(args, message);
    dstr buffer = FormatVa(message, args);
    va_end(args);

    return buffer;
}

dstr FormatVa(cstr message, va_list args)
{
    va_list copiedArgs;
    va_copy(copiedArgs, args);

    // why snprintf gotta return int but take size_t like what the fuck
    s32 size = vsnprintf(NULL, 0, message, copiedArgs) + 1;
    if (!size)
    {
        return 0;
    }

    va_end(copiedArgs);
    va_copy(copiedArgs, args);

    dstr buffer = new char[size];
    if (!buffer)
    {
        Error("failed to allocate memory in FormatVa");
    }

    vsnprintf(buffer, size, message, copiedArgs);
    va_end(copiedArgs);

    return buffer;
}

void Error(cstr message, ...)
{
    va_list args;
    va_start(args, message);
    // so FormatVa can report OOM
    char buffer[1024];
    vsnprintf(buffer, ARRAYSIZE(buffer), message, args);
    va_end(args);

    LogFatal("Fatal error: %s", buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal error", buffer, nullptr);
    abort();
}

void EcsLog(s32 Level, cstr File, s32 Line, cstr Message)
{
    LOG_LEVEL RealLevel;

    switch (Level)
    {
    case 0:
        RealLevel = LogLevelTrace;
        break;
    case -2:
        RealLevel = LogLevelWarning;
        break;
    case -3:
        RealLevel = LogLevelError;
        break;
    case -4:
        RealLevel = LogLevelFatal;
        break;
    default:
        RealLevel = LogLevelDebug;
        break;
    }

    RealLevel = LogLevelTrace;

    if (RealLevel == LogLevelFatal)
    {
        Error("ECS error at %s:%d: %s", File, Line, Message);
    }
    else
    {
        LogMessage(RealLevel, File, Line, false, "%s", Message);
    }
}

void *EcsMalloc(ecs_size_t size)
{
    return calloc(size, 1);
}

void EcsFree(void *block)
{
    free(block);
}

void *EcsRealloc(void *block, ecs_size_t size)
{
    return realloc(block, size);
}

void *EcsCalloc(ecs_size_t size)
{
    return calloc(1, size);
}
