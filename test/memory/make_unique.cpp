#include "gtest/gtest.h"
#include <string>

#include "sky/memory.hpp"

using namespace sky;

TEST(MakeUnique, IntType)
{
    auto p = make_unique<int>(20);

    EXPECT_EQ(20, *p);
}

TEST(MakeUnique, StringType)
{
    using std::string;
    auto p = make_unique<string>(5, 'x');

    EXPECT_EQ("xxxxx", *p);
}

TEST(MakeUnique, ArrayType)
{
    auto p = make_unique<int[]>(10, 20, 30);

    EXPECT_EQ(10, p[0]);
    EXPECT_EQ(20, p[1]);
    EXPECT_EQ(30, p[2]);
}

TEST(MakeUnique, NoExceptSpecifier)
{
    using std::string;
    EXPECT_TRUE (noexcept(make_unique<int>(0)));
    EXPECT_FALSE(noexcept(make_unique<string>("boom")));
}
