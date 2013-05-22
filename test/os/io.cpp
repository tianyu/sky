#include <stdexcept>
#include <unistd.h>

#include "gtest/gtest.h"

#include "sky/memory.hpp"
#include "sky/scope_guard.hpp"
#include "sky/os.h"

using namespace sky;

class IO : public ::testing::Test
{
public:
    IO()
    {
        int fds[2];
        assert(0 == ::pipe(fds));
        read_fd = fds[0];
        write_fd = fds[1];
    }

    ~IO()
    {
        ::close(read_fd);
        ::close(write_fd);
    }

public:
    int read_fd;
    int write_fd;
};

TEST_F(IO, ConstructOutput)
{
    output(0);
}

TEST_F(IO, OutputIsStandard)
{
    EXPECT_TRUE(output(1).is_standard());
    EXPECT_TRUE(output(2).is_standard());
}

TEST_F(IO, OutputIsNotStandard)
{
    EXPECT_FALSE(output(-1).is_standard());
    EXPECT_FALSE(output(3).is_standard());
}

TEST_F(IO, CloseOutput)
{
    ASSERT_FALSE(::close(read_fd));

    output out(write_fd);

    EXPECT_NO_THROW(out.close())
            << "Couldn't close the output.";
    EXPECT_ANY_THROW(out.close())
            << "Shouldn't be able to close an output more than once.";
}

TEST_F(IO, DupOutput)
{
    output out(100);
    auto close_out = scope_guard([&] { out.close(); });

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    out.dup(output(write_fd));

    ASSERT_EQ(6, out.write(expected));
    ASSERT_EQ(6, ::read(read_fd, actual, 10));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, DupOutput_Same)
{
    output out(write_fd);
    auto close_out = scope_guard([&] { out.close(); });

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    out.dup(output(write_fd));

    ASSERT_EQ(6, out.write(expected));
    ASSERT_EQ(6, ::read(read_fd, actual, 10));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, DupOutput_BadFd)
{
    output out(100);
    EXPECT_THROW(out.dup(output(-1)), std::invalid_argument);
}

TEST_F(IO, DupThisOutput)
{
    output out(write_fd);

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    output dup = out.dup();
    auto close_dup = scope_guard([&] { dup.close(); });

    ASSERT_EQ(6, dup.write(expected));
    ASSERT_EQ(6, ::read(read_fd, actual, 10));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, DupThisOutput_BadFd)
{
    output out(100);
    EXPECT_THROW(out.dup(), std::invalid_argument);
}

TEST_F(IO, CloseOutput_BadFile)
{
    output out(-1);
    EXPECT_THROW(out.close(), std::invalid_argument);
}

TEST_F(IO, ConstuctInput)
{
    input(1);
}

TEST_F(IO, InputIsStandard)
{
    EXPECT_TRUE(input(0).is_standard());
}

TEST_F(IO, InputIsNotStandard)
{
    EXPECT_FALSE(input(-1).is_standard());
    EXPECT_FALSE(input(3).is_standard());
}

TEST_F(IO, CloseInput)
{
    ASSERT_FALSE(::close(read_fd));

    input in(write_fd);

    EXPECT_NO_THROW(in.close())
            << "Couldn't close the input.";
    EXPECT_ANY_THROW(in.close())
            << "Shouldn't be able to close an input more than once.";
}

TEST_F(IO, CloseInput_BadFile)
{
    input in(-1);
    EXPECT_THROW(in.close(), std::invalid_argument);
}

TEST_F(IO, DupInput)
{
    input in(100);
    auto close_out = scope_guard([&] { in.close(); });

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    in.dup(input(read_fd));

    ASSERT_EQ(6, ::write(write_fd, expected, 6));
    ASSERT_EQ(6, in.read(actual));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, DupInput_Same)
{
    input in(read_fd);
    auto close_out = scope_guard([&] { in.close(); });

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    in.dup(input(read_fd));

    ASSERT_EQ(6, ::write(write_fd, expected, 6));
    ASSERT_EQ(6, in.read(actual));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, DupInput_BadFd)
{
    input in(100);
    EXPECT_THROW(in.dup(input(-1)), std::invalid_argument);
}

TEST_F(IO, DupThisInput)
{
    input in(read_fd);

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    input dup = in.dup();
    auto close_dup = scope_guard([&] { dup.close(); });

    ASSERT_EQ(6, ::write(write_fd, expected, 6));
    ASSERT_EQ(6, dup.read(actual));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, DupThisInput_BadFd)
{
    input in(100);
    EXPECT_THROW(in.dup(), std::invalid_argument);
}

TEST_F(IO, Write)
{
    output out(write_fd);

    char const*expected = "Hello";
    char actual[10] { '\0' };

    EXPECT_NO_THROW(out.write(expected, 6));

    ASSERT_EQ(6, ::read(read_fd, actual, 10));
    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, WriteArray)
{
    output out(write_fd);

    char const expected[] = "Hello";
    char actual[10] { '\0' };

    EXPECT_NO_THROW(out.write(expected));

    ASSERT_EQ(6, ::read(read_fd, actual, 10));
    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, WriteObject)
{
    output out(write_fd);

    double expected = 3.2;
    double actual = 0.0;

    EXPECT_NO_THROW(out.write(expected));

    ASSERT_EQ(sizeof(double), ::read(read_fd, &actual, sizeof(double)));
    EXPECT_FLOAT_EQ(expected, actual);
}

TEST_F(IO, Write_ReadFile)
{
    output out(read_fd);

    EXPECT_THROW(out.write("Hello World!", 1), std::invalid_argument);
}

TEST_F(IO, Write_ClosedFile)
{
    output out(write_fd);

    ASSERT_FALSE(::close(write_fd));
    EXPECT_THROW(out.write("Hello World!", 1), std::invalid_argument);
}

TEST_F(IO, Write_BadFile)
{
    output out(-1);
    EXPECT_THROW(out.write("Hello World!", 1), std::invalid_argument);
}

TEST_F(IO, Read)
{
    input in(read_fd);

    char const*expected = "Hello";
    char actual[10] { '\0' };

    ASSERT_EQ(6, ::write(write_fd, expected, 6));
    EXPECT_EQ(6, in.read(actual));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, ReadArray)
{
    input in(read_fd);

    char const*expected = "Hello";
    char actual[10] { '\0' };

    ASSERT_EQ(6, ::write(write_fd, expected, 6));
    EXPECT_EQ(6, in.read(actual));

    EXPECT_STREQ(expected, actual);
}

TEST_F(IO, ReadObject)
{
    input in(read_fd);

    double expected = 3.2;
    double actual = 0.0;

    ASSERT_EQ(sizeof(double), ::write(write_fd, &expected, sizeof(double)));
    EXPECT_EQ(sizeof(double), in.read(actual));

    EXPECT_FLOAT_EQ(expected, actual);
}

TEST_F(IO, Read_WriteFile)
{
    input in(write_fd);

    char buf[20] { '\0' };
    EXPECT_THROW(in.read(buf), std::invalid_argument);
}

TEST_F(IO, Read_ClosedFile)
{
    ASSERT_EQ(13, ::write(write_fd, "Hello World!", 13));

    input in(read_fd);

    char buf[20] { '\0' };
    ASSERT_FALSE(::close(read_fd));
    EXPECT_THROW(in.read(buf), std::invalid_argument);
}

TEST_F(IO, Read_BadFile)
{
    input in(-1);
    char buf[20] { '\0' };
    EXPECT_THROW(in.read(buf), std::invalid_argument);
}

TEST_F(IO, WriteRead)
{
    output out(write_fd);
    input in(read_fd);

    char const expected[] = "Hello World!";
    char actual[20] { '\0' };

    ASSERT_EQ(13, out.write(expected));
    ASSERT_EQ(13, in.read(actual));

    EXPECT_STREQ(expected, actual);
}
