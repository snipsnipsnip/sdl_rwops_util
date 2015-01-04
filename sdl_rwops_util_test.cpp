#include "sdl_rwops_util.h"
#include "gtest/gtest.h"

static SDL_RWops *makeRW(const char *text)
{
    return SDL_RWFromConstMem(text, strlen(text));
}

TEST(RWopsUtilTest, GetcEmpty)
{
    SDL_RWops *rw = makeRW("");
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST(RWopsUtilTest, GetcHoge)
{
    SDL_RWops *rw = makeRW("hoge");
    EXPECT_EQ('h', sdl_rwops_util_getc(rw));
    EXPECT_EQ('o', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('e', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST(RWopsUtilTest, GetcDoesntTouchCrLfs)
{
    SDL_RWops *rw = makeRW("\r\n");
    EXPECT_EQ('\r', sdl_rwops_util_getc(rw));
    EXPECT_EQ('\n', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));

    rw = makeRW("\r");
    EXPECT_EQ('\r', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));

    rw = makeRW("\n");
    EXPECT_EQ('\n', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST(RWopsUtilTest, GetcDoesntTouchCrBetweenLines)
{
    SDL_RWops *rw = makeRW("hoge\rfuga");
    EXPECT_EQ('h', sdl_rwops_util_getc(rw));
    EXPECT_EQ('o', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('e', sdl_rwops_util_getc(rw));
    EXPECT_EQ('\r', sdl_rwops_util_getc(rw));
    EXPECT_EQ('f', sdl_rwops_util_getc(rw));
    EXPECT_EQ('u', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('a', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST(RWopsUtilTest, GetcDoesntTouchLfBetweenLines)
{
    SDL_RWops *rw = makeRW("hoge \nfuga");
    EXPECT_EQ('h', sdl_rwops_util_getc(rw));
    EXPECT_EQ('o', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('e', sdl_rwops_util_getc(rw));
    EXPECT_EQ(' ', sdl_rwops_util_getc(rw));
    EXPECT_EQ('\n', sdl_rwops_util_getc(rw));
    EXPECT_EQ('f', sdl_rwops_util_getc(rw));
    EXPECT_EQ('u', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('a', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST(RWopsUtilTest, GetcDoesntTouchCrLfBetweenLines)
{
    SDL_RWops *rw = makeRW("hoge\r\nfuga");
    EXPECT_EQ('h', sdl_rwops_util_getc(rw));
    EXPECT_EQ('o', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('e', sdl_rwops_util_getc(rw));
    EXPECT_EQ('\r', sdl_rwops_util_getc(rw));
    EXPECT_EQ('\n', sdl_rwops_util_getc(rw));
    EXPECT_EQ('f', sdl_rwops_util_getc(rw));
    EXPECT_EQ('u', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('a', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST(RWopsUtilTest, GetsEmpty)
{
    SDL_RWops *rw = makeRW("");

    EXPECT_EQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_EQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_EQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsHoge)
{
    SDL_RWops *rw = makeRW("hoge");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsDoesntTouchSpaces)
{
    SDL_RWops *rw = makeRW(" ");
    EXPECT_STREQ(" ", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW(" \n a \n");
    EXPECT_STREQ(" ", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(" a ", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsStripsCrLf)
{
    SDL_RWops *rw = makeRW("\r");
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("\r\r\r");
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("\n");
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("\n\n\n");
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("\r\n\n\r");
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsStripsCrLfsBeforeEOF)
{
    SDL_RWops *rw = makeRW("hoge\r");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("hoge\n");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("hoge\r\n");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW("hoge\r\nfuga\r\n");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("fuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsCrIsNotNewline)
{
    SDL_RWops *rw = makeRW("hoge\rfuga\r\nmoga\nfoo");
    EXPECT_STREQ("hoge\rfuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("moga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("foo", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsEatsMultipleCrs)
{
    SDL_RWops *rw = makeRW("hoge\r\r\r\nfuga\r\nmoga");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("fuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("moga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, GetsEmptyLines)
{
    SDL_RWops *rw = makeRW("hoge\nfuga\n\r\n");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("fuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST(RWopsUtilTest, SlurpEmpty)
{
    SDL_RWops *rw = makeRW("");
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}

TEST(RWopsUtilTest, SlurpHoge)
{
    SDL_RWops *rw = makeRW("hoge");
    EXPECT_STREQ("hoge", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}

TEST(RWopsUtilTest, SlurpDoesntTouchCrLf)
{
    SDL_RWops *rw = makeRW("hoge\nfuga");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("fuga", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));

    rw = makeRW("hoge\rfuga");
    EXPECT_STREQ("hoge\rfuga", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));

    rw = makeRW("hoge\r\nfuga");
    EXPECT_EQ('h', sdl_rwops_util_getc(rw));
    EXPECT_STREQ("oge\r\nfuga", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}
