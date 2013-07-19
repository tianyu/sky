#include "gtest/gtest.h"

#include "common.hpp"

template<typename P>
struct Array_Capacity : public ::testing::Test
{};

using Types = ::testing::Types<
    IntParam<>,
    IntParam<2>,
    IntParam<2, 2>,
    IntParam<0>,
    IntParam<2, 0>
>;

TYPED_TEST_CASE(Array_Capacity, Types);

TYPED_TEST(Array_Capacity, Size_IsConstexpr)
{
    constexpr auto array = typename TypeParam::array_type{};
    constexpr auto size = array.size();
    (void) size;
}

TYPED_TEST(Array_Capacity, Size_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.size()));
}

TYPED_TEST(Array_Capacity, Size)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto size = array.size();

    EXPECT_EQ(expected, size);
}

TYPED_TEST(Array_Capacity, Max_Size_IsConstexpr)
{
    constexpr auto array = typename TypeParam::array_type{};
    constexpr auto max_size = array.max_size();
    (void) max_size;
}

TYPED_TEST(Array_Capacity, Max_Size_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.max_size()));
}

TYPED_TEST(Array_Capacity, Max_Size)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto max_size = array.max_size();

    EXPECT_EQ(expected, max_size);
}

TYPED_TEST(Array_Capacity, Empty_IsConstexpr)
{
    constexpr auto array = typename TypeParam::array_type{};
    constexpr auto empty = array.empty();
    (void) empty;
}

TYPED_TEST(Array_Capacity, Empty_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.empty()));
}

TYPED_TEST(Array_Capacity, Empty)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::empty;
    auto empty = array.empty();

    EXPECT_EQ(expected, empty);
}
