#include <unistd.h>
#include <stdexcept>

#include "gtest/gtest.h"

#include "sky/os.h"

using namespace sky;

TEST(Input, Construct)
{
    input(0);
}

TEST(Input, Close)
{
    int fds[2];
    EXPECT_FALSE(::pipe(fds));
    EXPECT_FALSE(::close(fds[1]));

    input in(fds[0]);

    EXPECT_NO_THROW(in.close())
            << "Couldn't close the input.";
    EXPECT_ANY_THROW(in.close())
            << "Shouldn't be able to close an input more than once.";
}

TEST(Input, Close_BadFile)
{
    input in(-1);
    EXPECT_THROW(in.close(), std::invalid_argument);
}

TEST(Output, Constuct)
{
    output(1);
}

TEST(Output, Close)
{
    int fds[2];
    EXPECT_FALSE(::pipe(fds));
    EXPECT_FALSE(::close(fds[0]));

    output out(fds[1]);

    EXPECT_NO_THROW(out.close())
            << "Couldn't close the output.";
    EXPECT_ANY_THROW(out.close())
            << "Shouldn't be able to close an output more than once.";
}

TEST(Output, Close_BadFile)
{
    output out(-1);
    EXPECT_THROW(out.close(), std::invalid_argument);
}
