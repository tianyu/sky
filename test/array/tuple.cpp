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

void expect_eq(int const&one, int const&two)
{
    EXPECT_EQ(one, two);
}

template<std::size_t... Ns>
void expect_eq(sky::array<int, Ns...> const&one,
               sky::array<int, Ns...> const&two)
{
    auto end_it = one.end();
    auto one_it = one.begin();
    auto two_it = two.begin();
    while (one_it != end_it) {
        expect_eq(*one_it++, *two_it++);
    }
}

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

TYPED_TEST(Array_AsTuple, Get_Lvalue)
{
    using std::get;

    auto array = TypeParam::make_array();

    using expected_type = typename TypeParam::row_type&;
    using return_type = decltype(get<0>(array));
    EXPECT_SAME(expected_type, return_type);

    constexpr auto halfway = std::size_t(TypeParam::num_rows/2);
    auto expected = array[halfway];
    auto actual = get<halfway>(array);
    expect_eq(expected, actual);
}
