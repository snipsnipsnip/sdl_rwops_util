#ifndef SDL_RWOPS_UTIL_H
#define SDL_RWOPS_UTIL_H

/*
 * sdl_rwops_util.c
 * Copyright (c) 2014 snipsnipsnip
 * zlib license
 */

#include "SDL_RWops.h"
#include "vstring.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SDL_RWops equivalent of getc().
 * returns -1 on failure or EOF.
 */
int sdl_rwops_util_getc(SDL_RWops *rw);

/**
 * SDL_RWops equivalent of (the infamous) gets().
 * Reads buffer till '\n' appears.
 * '\n' and '\r' at the end of the line (if any) will be removed.
 * Note that you won't get contents after '\0' since this function use C string.
 * returns NULL on failure or EOF.
 */
const char *sdl_rwops_util_gets(SDL_RWops *rw);

/**
 * Reads the whole remaining content from SDL_RWops.
 * Reads buffer till EOF appears.
 * Note that you won't get contents after '\0' since this function use C string.
 * returns NULL on failure or EOF.
 */
const char *sdl_rwops_util_slurp(SDL_RWops *rw);

/**
 * SDL_RWops equivalent of (the infamous) gets().
 * Reads buffer till '\n' appears.
 * '\n' and '\r' at the end of the line (if any) will be removed.
 * returns NULL on failure or EOF.
 */
vstring *sdl_rwops_util_gets_v(SDL_RWops *rw, vstring *s);

/**
 * Reads the whole remaining content from SDL_RWops.
 * Reads buffer till EOF appears.
 * returns NULL on failure or EOF.
 */
vstring *sdl_rwops_util_slurp_v(SDL_RWops *rw, vstring *s);

#ifdef __cplusplus
}
#endif

#endif /* !defined(SDL_RWOPS_UTIL_H) */
