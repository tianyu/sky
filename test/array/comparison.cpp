#include "gtest/gtest.h"

#include "sky/array.hpp"

#include "common.hpp"

namespace {

template<typename P>
struct Array_Comparison : public ::testing::Test
{};

using Types = ::testing::Types<
    IntParam<>,
    IntParam<2>,
    IntParam<2, 3>,
    IntParam<2, 3, 2>
>;

TYPED_TEST_CASE(Array_Comparison, Types);

} // namespace

TYPED_TEST(Array_Comparison, Equal)
{
    auto array1 = TypeParam::make_array();
    auto array2 = array1;

    EXPECT_TRUE(array1 == array2);
    EXPECT_FALSE(array1 != array2);
}

TYPED_TEST(Array_Comparison, NotEqual)
{
    auto array1 = TypeParam::make_array();
    auto array2 = array1;
    array2.back() += 1;

    EXPECT_FALSE(array1 == array2);
    EXPECT_TRUE(array1 != array2);
}
