#include <type_traits>

#include "../gtest.h"

#include "common.hpp"

// Base-case array access tests //
/*
  These tests only test the base case: the 0-dimensional array.
*/

using array_base = sky::array<int>;

TEST(Array_Access_Base, Operator)
{
    auto array = array_base{ 1 };
    EXPECT_EQ(1, array);
}

TEST(Array_Access_Base, Operator_Const)
{
    const auto array = array_base{ 1 };
    EXPECT_EQ(1, array);
}

TEST(Array_Access_Base, Operator_Rvalue)
{
    EXPECT_EQ(1, array_base{ 1 });
}

TEST(Array_Access_Base, At)
{
    auto array = array_base{ 1 };
    EXPECT_EQ(1, array.at());
}

TEST(Array_Access_Base, At_Const)
{
    const auto array = array_base{ 1 };
    EXPECT_EQ(1, array.at());
}

TEST(Array_Access_Base, At_Rvalue)
{
    EXPECT_EQ(1, array_base{ 1 }.at());
}

// General array access tests //

template<typename P>
struct Array_Access : public ::testing::Test
{};

using Types = ::testing::Types<
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

TYPED_TEST(Array_Access, Operator_Const)
{
    const auto array = TypeParam::make_array();
    EXPECT_SAME(typename TypeParam::const_row_type, decltype(array[0]));

    int expected = 1;
    for (std::size_t i = 0; i < TypeParam::num_rows; ++i) {
        auto row = array[i];
        EXPECT_EQ(expected, front_of(row));
        expected += TypeParam::row_size;
    }
}

TYPED_TEST(Array_Access, At)
{
    auto array = TypeParam::make_array();
    EXPECT_SAME(typename TypeParam::row_type, decltype(array.at(0)));

    int expected = 1;
    for (std::size_t i = 0; i < TypeParam::num_rows; ++i) {
        auto row = array.at(i);
        EXPECT_EQ(expected, front_of(row));
        expected += TypeParam::row_size;
    }
}

TYPED_TEST(Array_Access, At_Const)
{
    const auto array = TypeParam::make_array();
    EXPECT_SAME(typename TypeParam::const_row_type, decltype(array.at(0)));

    int expected = 1;
    for (std::size_t i = 0; i < TypeParam::num_rows; ++i) {
        auto row = array.at(i);
        EXPECT_EQ(expected, front_of(row));
        expected += TypeParam::row_size;
    }
}
