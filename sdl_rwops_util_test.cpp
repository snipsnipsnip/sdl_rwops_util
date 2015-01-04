#include "sdl_rwops_util.h"
#include "gtest/gtest.h"
#include <string>

extern "C" int disabled_seek(SDL_RWops *, int, int)
{
    return -1;
}

class RWopsUtilTest : public ::testing::TestWithParam<bool>
{
protected:
    template <size_t Size>
    SDL_RWops *makeRW(const char (&text)[Size])
    {
        SDL_RWops *rw = SDL_RWFromConstMem(text, Size - 1);
    
        if (GetParam())
        {
            rw->seek = &disabled_seek;
        }

        return rw;
    }
};

INSTANTIATE_TEST_CASE_P(SeekableOrNot, RWopsUtilTest, testing::Values(false, true));

TEST_P(RWopsUtilTest, GetcEmpty)
{
    SDL_RWops *rw = makeRW("");
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST_P(RWopsUtilTest, GetcHoge)
{
    SDL_RWops *rw = makeRW("hoge");
    EXPECT_EQ('h', sdl_rwops_util_getc(rw));
    EXPECT_EQ('o', sdl_rwops_util_getc(rw));
    EXPECT_EQ('g', sdl_rwops_util_getc(rw));
    EXPECT_EQ('e', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST_P(RWopsUtilTest, GetcDoesntTouchCrLfs)
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

TEST_P(RWopsUtilTest, GetcDoesntTouchCrBetweenLines)
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

TEST_P(RWopsUtilTest, GetcDoesntTouchLfBetweenLines)
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

TEST_P(RWopsUtilTest, GetcDoesntTouchCrLfBetweenLines)
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

TEST_P(RWopsUtilTest, GetcGoesAfterNulBytes)
{
    SDL_RWops *rw = makeRW("Lorem\0Ipsum");
    
    EXPECT_EQ('L', sdl_rwops_util_getc(rw));
    EXPECT_EQ('o', sdl_rwops_util_getc(rw));
    EXPECT_EQ('r', sdl_rwops_util_getc(rw));
    EXPECT_EQ('e', sdl_rwops_util_getc(rw));
    EXPECT_EQ('m', sdl_rwops_util_getc(rw));
    EXPECT_EQ('\0', sdl_rwops_util_getc(rw));
    EXPECT_EQ('I', sdl_rwops_util_getc(rw));
    EXPECT_EQ('p', sdl_rwops_util_getc(rw));
    EXPECT_EQ('s', sdl_rwops_util_getc(rw));
    EXPECT_EQ('u', sdl_rwops_util_getc(rw));
    EXPECT_EQ('m', sdl_rwops_util_getc(rw));
    EXPECT_EQ(EOF, sdl_rwops_util_getc(rw));
}

TEST_P(RWopsUtilTest, GetsEmpty)
{
    SDL_RWops *rw = makeRW("");

    EXPECT_EQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_EQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_EQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, GetsHoge)
{
    SDL_RWops *rw = makeRW("hoge");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, GetsDoesntTouchSpaces)
{
    SDL_RWops *rw = makeRW(" ");
    EXPECT_STREQ(" ", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));

    rw = makeRW(" \n a \n");
    EXPECT_STREQ(" ", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(" a ", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, GetsStripsCrLf)
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

TEST_P(RWopsUtilTest, GetsStripsCrLfsBeforeEOF)
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

TEST_P(RWopsUtilTest, GetsCrIsNotNewline)
{
    SDL_RWops *rw = makeRW("hoge\rfuga\r\nmoga\nfoo");
    EXPECT_STREQ("hoge\rfuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("moga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("foo", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, GetsEatsMultipleCrs)
{
    SDL_RWops *rw = makeRW("hoge\r\r\r\nfuga\r\nmoga");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("fuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("moga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, GetsEmptyLines)
{
    SDL_RWops *rw = makeRW("hoge\nfuga\n\r\n");
    EXPECT_STREQ("hoge", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("fuga", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("", sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, GetsDoesReadNulBytes)
{
    SDL_RWops *rw = makeRW("Lorem\0Ipsum");
    
    EXPECT_EQ(std::string("Lorem\0Ipsum", 12), std::string(sdl_rwops_util_gets(rw), 12));
    EXPECT_STREQ(NULL, sdl_rwops_util_gets(rw));
}

TEST_P(RWopsUtilTest, SlurpEmpty)
{
    SDL_RWops *rw = makeRW("");
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}

TEST_P(RWopsUtilTest, SlurpHoge)
{
    SDL_RWops *rw = makeRW("hoge");
    EXPECT_STREQ("hoge", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}

TEST_P(RWopsUtilTest, SlurpDoesReadNulBytes)
{
    SDL_RWops *rw = makeRW("Lorem\0Ipsum");
    
    EXPECT_EQ(std::string("Lorem\0Ipsum", 12), std::string(sdl_rwops_util_slurp(rw), 12));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}

TEST_P(RWopsUtilTest, SlurpDoesntTouchCrLf)
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

TEST_P(RWopsUtilTest, SlurpLoremIpsum)
{
    SDL_RWops *rw = makeRW("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    EXPECT_STREQ("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}

TEST_P(RWopsUtilTest, SlurpAndGets)
{
    SDL_RWops *rw = makeRW("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    
    EXPECT_STREQ("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.", sdl_rwops_util_gets(rw));
    EXPECT_STREQ("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", sdl_rwops_util_slurp(rw));
    EXPECT_STREQ(NULL, sdl_rwops_util_slurp(rw));
}
