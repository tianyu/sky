#include <stdexcept>
#include <unistd.h>

#include "gtest/gtest.h"

#include "sky/memory.hpp"
#include "sky/os.h"

using namespace sky;

TEST(IO, ConstructInput)
{
    input(0);
}

TEST(IO, CloseInput)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));
    ASSERT_FALSE(::close(fds[1]));

    input in(fds[0]);

    EXPECT_NO_THROW(in.close())
            << "Couldn't close the input.";
    EXPECT_ANY_THROW(in.close())
            << "Shouldn't be able to close an input more than once.";
}

TEST(IO, CloseInput_BadFile)
{
    input in(-1);
    EXPECT_THROW(in.close(), std::invalid_argument);
}

TEST(IO, ConstuctOutput)
{
    output(1);
}

TEST(IO, CloseOutput)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));
    ASSERT_FALSE(::close(fds[0]));

    output out(fds[1]);

    EXPECT_NO_THROW(out.close())
            << "Couldn't close the output.";
    EXPECT_ANY_THROW(out.close())
            << "Shouldn't be able to close an output more than once.";
}

TEST(IO, CloseOuput_BadFile)
{
    output out(-1);
    EXPECT_THROW(out.close(), std::invalid_argument);
}

TEST(IO, Write)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    input in(fds[1]);

    char const*expected = "Hello";
    char actual[10] { '\0' };

    EXPECT_NO_THROW(in.write(expected, 6));

    ASSERT_EQ(6, ::read(fds[0], actual, 10));
    EXPECT_STREQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, WriteArray)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    input in(fds[1]);

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    EXPECT_NO_THROW(in.write(expected));

    ASSERT_EQ(6, ::read(fds[0], actual, 10));
    EXPECT_STREQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, WriteObject)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    input in(fds[1]);

    double expected = 3.2;
    double actual = 0.0;

    EXPECT_NO_THROW(in.write(expected));

    ASSERT_EQ(sizeof(double), ::read(fds[0], &actual, sizeof(double)));
    EXPECT_FLOAT_EQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, Write_ReadFile)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    input in(fds[0]);

    EXPECT_THROW(in.write("Hello World!", 1), std::invalid_argument);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, Write_ClosedFile)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    input in(fds[1]);

    ASSERT_FALSE(::close(fds[1]));
    EXPECT_THROW(in.write("Hello World!", 1), std::invalid_argument);

    ASSERT_FALSE(::close(fds[0]));
}

TEST(IO, Write_BadFile)
{
    input in(-1);
    EXPECT_THROW(in.write("Hello World!", 1), std::invalid_argument);
}

TEST(IO, Read)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    output out(fds[0]);

    char const*expected = "Hello";
    char actual[10] { '\0' };

    ASSERT_EQ(6, ::write(fds[1], expected, 6));
    out.read(actual);

    EXPECT_STREQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, ReadArray)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    output out(fds[0]);

    char const*expected = "Hello";
    char actual[10] { '\0' };

    ASSERT_EQ(6, ::write(fds[1], expected, 6));
    out.read(actual);

    EXPECT_STREQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, ReadObject)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    output out(fds[0]);

    double expected = 3.2;
    double actual = 0.0;

    ASSERT_EQ(sizeof(double), ::write(fds[1], &expected, sizeof(double)));
    out.read(actual);

    EXPECT_FLOAT_EQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, Read_WriteFile)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    output out(fds[1]);

    char buf[20] { '\0' };
    EXPECT_THROW(out.read(buf), std::invalid_argument);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, Read_ClosedFile)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));
    ASSERT_EQ(13, ::write(fds[1], "Hello World!", 13));

    output out(fds[0]);

    char buf[20] { '\0' };
    ASSERT_FALSE(::close(fds[0]));
    EXPECT_THROW(out.read(buf), std::invalid_argument);

    ASSERT_FALSE(::close(fds[1]));
}

TEST(IO, Read_BadFile)
{
    output out(-1);
    char buf[20] { '\0' };
    EXPECT_THROW(out.read(buf), std::invalid_argument);
}

TEST(IO, WriteRead)
{
    int fds[2];
    ASSERT_FALSE(::pipe(fds));

    input in(fds[1]);
    output out(fds[0]);

    char const expected[] = "Hello World!";
    char actual[20] { '\0' };

    in.write(expected);
    out.read(actual);

    EXPECT_STREQ(expected, actual);

    ASSERT_FALSE(::close(fds[0]));
    ASSERT_FALSE(::close(fds[1]));
}
