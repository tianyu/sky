#include <type_traits>

#include "../gtest.h"

#include "common.hpp"

template<typename P>
struct Array_Access : public ::testing::Test
{};

using Types = ::testing::Types<
    Param<>,
    Param<2>,
    Param<2, 2>
>;

template<typename T>
int const&front_of(T const&array)
{
    return *reinterpret_cast<int const*>(&array);
}

TYPED_TEST_CASE(Array_Access, Types);

TYPED_TEST(Array_Access, Operator)
{
    auto array = TypeParam::make_array();
    EXPECT_SAME(typename TypeParam::row_type, decltype(array[0]));

    int expected = 1;
    for (std::size_t i = 0; i < TypeParam::num_rows; ++i) {
        auto row = array[i];
        EXPECT_EQ(expected, front_of(row));
        expected += TypeParam::row_size;
    }
}
