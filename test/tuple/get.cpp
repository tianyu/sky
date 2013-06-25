#include "gtest/gtest.h"

#include "sky/tuple.hpp"

using namespace sky;

TEST(Get, Get)
{
    auto t = std::make_tuple(long(1), int(5), long(8));
    int &x = sky::get<int>(t);
    EXPECT_EQ(5, x);
}

TEST(Get, Get_Const)
{
    const auto t = std::make_tuple(long(1), int(5), long(8));
    const int &x = sky::get<int>(t);
    EXPECT_EQ(5, x);
}

TEST(Get, IsRef)
{
    auto t = std::make_tuple(long(1), int(5), long(8));
    int &x = sky::get<int>(t);

    std::get<1>(t) = 10;
    EXPECT_EQ(10, x);
}

TEST(Get, IsRef_Const)
{
    auto t = std::make_tuple(long(1), int(5), long(8));
    const int &x = sky::get<int>((const decltype(t)&)t);

    std::get<1>(t) = 10;
    EXPECT_EQ(10, x);
}

TEST(Get, RValue)
{
    int &&x = sky::get<int>(std::make_tuple(long(1), int(5), long(8)));
    EXPECT_EQ(5, x);
}
