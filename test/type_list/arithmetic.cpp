#include "gtest/gtest.h"

#include "sky/type_list.hpp"

using sky::product;

template<int... Is>
using list_t = sky::type_list<int, Is...>;

TEST(Product, Empty)
{
    auto actual = product<list_t<>>::value;
    EXPECT_EQ(1, actual);
}

TEST(Product, One)
{
    auto actual = product<list_t<3>>::value;
    EXPECT_EQ(3, actual);
}

TEST(Product, Many)
{
    auto actual = product<list_t<7, 5, 2, 3>>::value;
    EXPECT_EQ(210, actual);
}
