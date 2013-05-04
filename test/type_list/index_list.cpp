#include <type_traits>

#include "gtest/gtest.h"

#include "sky/type_list.hpp"

using sky::index_list;
using sky::index_range;

namespace {

template<typename T, typename U>
void expect_same()
{
    bool is_same = std::is_same<T,U>::value;
    EXPECT_TRUE(is_same);
}

} // namespace

TEST(IndexRange, From_0_to_0)
{
    using expected = index_list<>;
    using actual = typename index_range<0,0>::type;

    expect_same<expected, actual>();
}

TEST(IndexRange, From_3_to_3)
{
    using expected = index_list<>;
    using actual = typename index_range<3,3>::type;

    expect_same<expected, actual>();
}

TEST(IndexRange, From_0_to_5)
{
    using expected = index_list<0,1,2,3,4>;
    using actual = typename index_range<0,5>::type;

    expect_same<expected, actual>();
}

TEST(IndexRange, From_3_to_7)
{
    using expected = index_list<3,4,5,6>;
    using actual = typename index_range<3,7>::type;

    expect_same<expected, actual>();
}
