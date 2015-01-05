#include "sdl_rwops_util.h"
#include "vstring.h"
#include "SDL_RWops.h"

/*
 * sdl_rwops_util.c
 * Copyright (c) 2014 snipsnipsnip
 * zlib license
 */

int sdl_rwops_util_getc(SDL_RWops *rw)
{
    unsigned char c;
    return SDL_RWread(rw, &c, 1, 1) == 1 ? c : EOF;
}

vstring *sdl_rwops_util_gets_v(SDL_RWops *rw, vstring *s)
{
    Uint16 initial_pointer = s->pointer;

    for (;;)
    {
        int c = sdl_rwops_util_getc(rw);

        if (c == EOF)
        {
            if (s->pointer == initial_pointer)
            {
                return NULL;
            }

            break;
        }

        if (c == '\n')
        {
            break;
        }

        if (!vs_push(s, c))
        {
            return NULL;
        }
    }

    while (s->pointer > initial_pointer && (s->contents[s->pointer - 1] == '\r' || s->contents[s->pointer - 1] == '\n'))
    {
        s->pointer--;
    }

    return s;
}

static int try_get_size(SDL_RWops *rw)
{
    int i = SDL_RWseek(rw, 0, RW_SEEK_CUR);
    int size;

    if (i < 0)
    {
        return -1;
    }

    size = SDL_RWseek(rw, 0, RW_SEEK_END);
    SDL_RWseek(rw, i, RW_SEEK_SET);

    return size;
}

vstring *sdl_rwops_util_slurp_v(SDL_RWops *rw, vstring *s)
{
    Uint64 initial_pointer = s->pointer;
    int rwsize = try_get_size(rw);

    if (rwsize > 0 && s->size - s->pointer < (Uint64)rwsize && !vs_resize(s, s->pointer + rwsize))
    {
        return NULL;
    }

    for (;;)
    {
        int read;

        if (rwsize < 0 && s->size == s->pointer && vs_resize(s, s->size + 1) == NULL)
        {
            return NULL;
        }

        read = SDL_RWread(rw, s->contents + s->pointer, 1, s->size - s->pointer);

        if (read <= 0)
        {
            if (s->pointer == initial_pointer)
            {
                return NULL;
            }

            break;
        }

        s->pointer += read;
    }

    return s;
}

const char *sdl_rwops_util_gets(SDL_RWops *rw)
{
    vstring s;
    vstring_malloc m = { SDL_malloc, SDL_realloc, SDL_free };

    if (vs_init(&s, &m, VS_TYPE_DYNAMIC, NULL, 1) == NULL)
    {
        return NULL;
    }
    if (sdl_rwops_util_gets_v(rw, &s) == NULL)
    {
        return NULL;
    }

    if (!vs_finalize(&s))
    {
        return NULL;
    }

    return s.contents;
}

const char *sdl_rwops_util_slurp(SDL_RWops *rw)
{
    vstring s;
    vstring_malloc m = { SDL_malloc, SDL_realloc, SDL_free };

    if (vs_init(&s, &m, VS_TYPE_DYNAMIC, NULL, 1) == NULL)
    {
        return NULL;
    }

    if (sdl_rwops_util_slurp_v(rw, &s) == NULL)
    {
        return NULL;
    }

    if (!vs_finalize(&s))
    {
        return NULL;
    }

    return s.contents;
}
