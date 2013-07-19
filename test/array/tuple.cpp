#include "../gtest.h"

#include <vector>

#include "common.hpp"

namespace {

template<typename P>
struct Array_AsTuple : public ::testing::Test
{};

using Types = ::testing::Types<
    IntParam<2>,
    IntParam<2, 3>,
    IntParam<2, 0>,
    IntParam<2, 3, 2>
>;

TYPED_TEST_CASE(Array_AsTuple, Types);

} // namespace

TYPED_TEST(Array_AsTuple, Tuple_Size)
{
    auto tuple_size = std::tuple_size<typename TypeParam::array_type>::value;
    EXPECT_EQ((TypeParam::num_rows), tuple_size);
}

TYPED_TEST(Array_AsTuple, Tuple_Element)
{
    using tuple_element = typename std::tuple_element<0, typename TypeParam::array_type>::type;
    using expected_type = typename TypeParam::row_type;
    EXPECT_SAME(expected_type, tuple_element);
}
