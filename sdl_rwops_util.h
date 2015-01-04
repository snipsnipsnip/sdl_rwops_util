/*
 * sdl_rwops_util
 * Copyright (c) 2014 snipsnipsnip
 * zlib license
 */

#ifndef SDL_RWOPS_UTIL_H_
#define SDL_RWOPS_UTIL_H_ 1

#include "SDL/SDL_RWops.h"
#include "vstring.h"

#ifdef __cplusplus
extern "C" {
#endif

int sdl_rwops_util_getc(SDL_RWops *rw);
const char *sdl_rwops_util_gets(SDL_RWops *rw);
const char *sdl_rwops_util_slurp(SDL_RWops *rw);

vstring *sdl_rwops_util_gets_v(SDL_RWops *rw, vstring *s);
vstring *sdl_rwops_util_slurp_v(SDL_RWops *rw, vstring *s);

#ifdef __cplusplus
}
#endif

#endif /* !defined(SDL_RWOPS_UTIL_H_) */
