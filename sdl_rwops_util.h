#ifndef SDL_RWOPS_UTIL_H
#define SDL_RWOPS_UTIL_H

/*
 * sdl_rwops_util.c
 * Copyright (c) 2014 snipsnipsnip
 * zlib license
 */

#include "SDL_RWops.h"
#include "vstring.h"

/* define SDL_RWOPS_UTIL_DLL to optimize for DLL */
#ifdef SDL_RWOPS_UTIL_DLL
#  ifdef SDL_RWOPS_UTIL_BUILDING
/* We are building this library */
#    define SDL_RWOPS_UTIL_API __declspec(dllexport)
#  else
/* We are using this library */
#    define SDL_RWOPS_UTIL_API __declspec(dllimport)
#  endif
#else
#  define SDL_RWOPS_UTIL_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SDL_RWops equivalent of getc().
 * returns -1 on failure or EOF.
 */
SDL_RWOPS_UTIL_API
int sdl_rwops_util_getc(SDL_RWops *rw);

/**
 * SDL_RWops equivalent of (the infamous) gets().
 * Reads buffer till '\n' appears.
 * '\n' and '\r' at the end of the line (if any) will be removed.
 * Note that you won't get contents after '\0' since this function use C string.
 * Returns string allocated by SDL_malloc().
 * Returns NULL on failure or EOF.
 */
SDL_RWOPS_UTIL_API
const char *sdl_rwops_util_gets(SDL_RWops *rw);

/**
 * Reads the whole remaining content from SDL_RWops.
 * Reads buffer till EOF appears.
 * Note that you won't get contents after '\0' since this function use C string.
 * Returns string allocated by SDL_malloc().
 * Returns NULL on failure or EOF.
 */
SDL_RWOPS_UTIL_API
const char *sdl_rwops_util_slurp(SDL_RWops *rw);

/**
 * SDL_RWops equivalent of (the infamous) gets().
 * Reads buffer till '\n' appears.
 * '\n' and '\r' at the end of the line (if any) will be removed.
 * returns NULL on failure or EOF.
 */
SDL_RWOPS_UTIL_API
vstring *sdl_rwops_util_gets_v(SDL_RWops *rw, vstring *s);

/**
 * Reads the whole remaining content from SDL_RWops.
 * Reads buffer till EOF appears.
 * returns NULL on failure or EOF.
 */
SDL_RWOPS_UTIL_API
vstring *sdl_rwops_util_slurp_v(SDL_RWops *rw, vstring *s);

#ifdef __cplusplus
}
#endif

#endif /* !defined(SDL_RWOPS_UTIL_H) */
