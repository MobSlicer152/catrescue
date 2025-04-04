/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 *
 * Modifications made by Randomcode Developers:
 * - NT code style
 * - Add option to display line number in hex (for callbacks where you get an
 * address)
 * - Add thread name to messages
 */

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "game.h"

#define LOG_USE_COLOR 1
#define LOG_VERSION "0.1.0"

typedef enum LOG_LEVEL
{
    LogLevelTrace,
    LogLevelDebug,
    LogLevelInfo,
    LogLevelWarning,
    LogLevelError,
    LogLevelFatal
} LOG_LEVEL;

typedef struct LOG_EVENT
{
    va_list ArgList;
    const char *Format;
    const char *File;
    struct tm *Time;
    void *Data;
    int64_t Line;
    bool HexLine;
    LOG_LEVEL Level;
} LOG_EVENT;

typedef void (*PFN_LOG_LOG)(LOG_EVENT *Event);
typedef void (*PFN_LOG_LOCK)(bool Lock, void *Data);

#define LogTrace(...) LogMessage(LogLevelTrace, __FILE__, __LINE__, false, __VA_ARGS__)
#define LogDebug(...) LogMessage(LogLevelDebug, __FILE__, __LINE__, false, __VA_ARGS__)
#define LogInfo(...) LogMessage(LogLevelInfo, __FILE__, __LINE__, false, __VA_ARGS__)
#define LogWarning(...) LogMessage(LogLevelWarning, __FILE__, __LINE__, false, __VA_ARGS__)
#define LogError(...) LogMessage(LogLevelError, __FILE__, __LINE__, false, __VA_ARGS__)
#define LogFatal(...) LogMessage(LogLevelFatal, __FILE__, __LINE__, false, __VA_ARGS__)

extern const char *LogGetLevelString(LOG_LEVEL Level);

extern void LogSetLock(PFN_LOG_LOCK Lock, void *Data);

extern void LogSetLevel(LOG_LEVEL Level);

extern LOG_LEVEL LogGetLevel(void);

extern void LogSetQuiet(bool Quiet);

extern int LogAddCallback(PFN_LOG_LOG Callback, void *Data, LOG_LEVEL Level);

extern int LogAddFile(FILE *File, LOG_LEVEL Level);

extern void LogMessage(LOG_LEVEL Level, const char *File, uint64_t Line, bool HexLine, const char *Format, ...);

#endif
