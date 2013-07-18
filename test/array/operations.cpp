#include "gtest/gtest.h"

#include "common.hpp"

namespace {

template<typename P>
struct Array_Operation : public ::testing::Test
{};

using Types = ::testing::Types<
    Param<>,
    Param<2>,
    Param<2, 3>,
    Param<2, 0>,
    Param<2, 3, 2>,
    Param<2, 0, 2>
>;

TYPED_TEST_CASE(Array_Operation, Types);

} // namespace

TYPED_TEST(Array_Operation, Fill)
{
    auto array = typename TypeParam::array_type{};
    array.fill(3);

    for (auto &value : array) {
        EXPECT_EQ(3, value);
    }
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
