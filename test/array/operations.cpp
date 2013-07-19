#include "gtest/gtest.h"

#include <vector>

#include "common.hpp"

namespace {

template<typename P>
struct Array_Operation : public ::testing::Test
{};

using Types = ::testing::Types<
    IntParam<>,
    IntParam<2>,
    IntParam<2, 3>,
    IntParam<2, 0>,
    IntParam<2, 3, 2>
>;

TYPED_TEST_CASE(Array_Operation, Types);

template<typename P>
struct Array_Operation_NotNoExcept : public ::testing::Test
{};

using NotNoExceptTypes = ::testing::Types<
    Param<std::vector<int>>,
    Param<std::vector<int>, 2>,
    Param<std::vector<int>, 2, 3>,
    Param<std::vector<int>, 2, 3, 2>
>;

TYPED_TEST_CASE(Array_Operation_NotNoExcept, NotNoExceptTypes);

} // namespace

TYPED_TEST(Array_Operation, Fill)
{
    auto array = typename TypeParam::array_type{};
    array.fill(3);

    for (auto &value : array) {
        EXPECT_EQ(3, value);
    }
}

TYPED_TEST(Array_Operation_NotNoExcept, Swap)
{
    auto array = typename TypeParam::array_type{};
    auto other = array;
    EXPECT_FALSE(noexcept(array.swap(other)));
}

TYPED_TEST(Array_Operation, Swap_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.swap(array)));
}

TYPED_TEST(Array_Operation, Swap)
{
    auto array1 = typename TypeParam::array_type{};
    auto array2 = TypeParam::make_array();

    array1.swap(array2);

    int expected = 1;
    for (auto &value : array1) {
        EXPECT_EQ(expected, value);
        expected++;
    }

    for (auto &value : array2) {
        EXPECT_EQ(0, value);
    }
}

TYPED_TEST(Array_Operation, NonMember_Swap)
{
    auto array1 = typename TypeParam::array_type{};
    auto array2 = TypeParam::make_array();

    std::swap(array1, array2);

    int expected = 1;
    for (auto &value : array1) {
        EXPECT_EQ(expected, value);
        expected++;
    }

    for (auto &value : array2) {
        EXPECT_EQ(0, value);
    }
}

