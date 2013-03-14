#include "gtest/gtest.h"

#include "sky/type_list.hpp"

using sky::type_list;

template <typename T, typename U>
static void expect_eq()
{
    bool is_same = std::is_same<T,U>::value;
    EXPECT_TRUE(is_same);
}

TEST(TypeList, Empty)
{
    typedef type_list<> list;
    EXPECT_TRUE(list::empty);
}

TEST(TypeList, First)
{
    typedef type_list<int> list;
    EXPECT_FALSE(list::empty);
    expect_eq<int, list::first>();
}

TEST(TypeList, Rest)
{
    typedef type_list<int, long> list;
    EXPECT_FALSE(list::empty);
    expect_eq<int, list::first>();

    typedef typename list::rest rest;
    expect_eq<long, rest::first>;

    typedef typename rest::rest rest_rest;
    EXPECT_TRUE(rest_rest::empty);
}
