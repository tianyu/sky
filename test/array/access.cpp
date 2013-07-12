#include <type_traits>

#include "../gtest.h"

#include "common.hpp"

// Base-case array access tests //
/*
  These tests only test the base case: the 0-dimensional array.
*/

using array_base = sky::array<int>;

TEST(Array_Access_Base, Operator_IsNoExcept)
{
    auto array = array_base { 1 };
    EXPECT_TRUE(noexcept(int(array)));
}

TEST(Array_Access_Base, Operator_Const_IsNoExcept)
{
    const auto array = array_base { 1 };
    EXPECT_TRUE(noexcept(int(array)));
}

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

TEST(Array_Access_Base, At_IsNoExcept)
{
    auto array = array_base { 1 };
    EXPECT_TRUE(noexcept(array.at()));
}

TEST(Array_Access_Base, At_Const_IsNoExcept)
{
    const auto array = array_base { 1 };
    EXPECT_TRUE(noexcept(array.at()));
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
    Param<2, 2>,
    Param<2, 3, 3>
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

TYPED_TEST(Array_Access, At_NoIndex_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.at()));
}

TYPED_TEST(Array_Access, At_NoIndex_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.at()));
}

TYPED_TEST(Array_Access, At_NoIndex)
{
    auto array = TypeParam::make_array();
    auto &ref = array;
    EXPECT_SAME(decltype((ref)), decltype(array.at()));
    EXPECT_EQ(&array, &array.at());
}

TYPED_TEST(Array_Access, At_NoIndex_Const)
{
    const auto array = TypeParam::make_array();
    auto &ref = array;
    EXPECT_SAME(decltype((ref)), decltype(array.at()));
    EXPECT_EQ(&array, &array.at());
}

TYPED_TEST(Array_Access, At_OneIndex)
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

TYPED_TEST(Array_Access, At_OneIndex_Const)
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

TYPED_TEST(Array_Access, At_OutOfRange)
{
    auto array = TypeParam::make_array();
    auto bad_index = TypeParam::num_rows;
    EXPECT_THROW(array.at(bad_index), std::out_of_range);
}

TYPED_TEST(Array_Access, At_OutOfRange_Const)
{
    const auto array = TypeParam::make_array();
    auto bad_index = TypeParam::num_rows;
    EXPECT_THROW(array.at(bad_index), std::out_of_range);
}
