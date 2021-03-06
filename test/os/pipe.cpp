#include "gtest/gtest.h"

#include "sky/scope_guard.hpp"

#include "sky/os.h"

using namespace sky;

TEST(Pipe, Make)
{
    auto pipe = make_pipe();
    input &in = std::get<0>(pipe);
    output &out = std::get<1>(pipe);
    auto close_in = scope_guard([&] { in.close(); });
    auto close_out = scope_guard([&] { out.close(); });

    char const expected[] { "Hello" };
    char actual[10] { '\0' };

    ASSERT_EQ(6, out.write(expected));
    ASSERT_EQ(6, in.read(actual));

    EXPECT_STREQ(expected, actual);
}
