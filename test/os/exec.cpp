#include "gtest/gtest.h"

#include "sky/os.h"

using namespace sky;

constexpr const auto ls = cmd("ls", "-l");

TEST(Cmd, Is_Executable)
{
    EXPECT_TRUE(is_executable<_::cmd<0>>::value);
    EXPECT_TRUE(is_executable<_::cmd<1>>::value);
    EXPECT_TRUE(is_executable<_::cmd<3>>::value);
}

TEST(Cmd, Construct_0Args)
{
    _::cmd<0>("ls");
}

TEST(Cmd, Construct_1Args)
{
    _::cmd<1>("ls", "-l");
}

TEST(Cmd, Construct_3Args)
{
    _::cmd<3>("ls", "-a", "-l", "*");
}

TEST(Cmd, Make_0Args)
{
    cmd("ls");
}

TEST(Cmd, Make_1Args)
{
    cmd("ls", "-l");
}

TEST(Cmd, Make_3Args)
{
    cmd("ls", "-a", "-l", "*");
}
