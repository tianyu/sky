#include <array>
#include <iterator>
#include <type_traits>

#include "gtest/gtest.h"

#include "sky/type_list.hpp"

#include "sky/array.hpp"

namespace  {

template<std::size_t... Ns> struct Param
{
private:
    template<std::size_t... Vals>
    static std::array<int, sizeof...(Vals)> &&
    make_array_helper(sky::index_list<Vals...>)
    {
        return std::array<int, sizeof...(Vals)>{{ Vals... }};
    }

public:

    static const std::size_t size = sky::product<sky::index_list<Ns...>>::value;

    static const bool is_empty = (size == 0);

    static sky::array<int, Ns...> make_array()
    {
        using values_t = typename sky::index_range<1, size + 1>::type;
        // This reinterpret cast should work since both sky::array and
        // std::array should have the same layout.
        return reinterpret_cast<sky::array<int, Ns...>&&>(
                    make_array_helper(values_t()));
    }

};

template<typename P>
struct Array_Member : public ::testing::Test
{};

template<typename T>
constexpr bool is_const(T &) { return false; }

template<typename T>
constexpr bool is_const(T const&) { return true; }

} // namespace

using Types = ::testing::Types<
    Param<>,
    Param<2>,
    Param<2, 2>,
    Param<0>,
    Param<2, 0>
>;

TYPED_TEST_CASE(Array_Member, Types);

TYPED_TEST(Array_Member, Data_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.data()));
}

TYPED_TEST(Array_Member, Data_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.data()));
}

TYPED_TEST(Array_Member, Data)
{
    auto array = TypeParam::make_array();
    auto data = array.data();

    if (TypeParam::is_empty) {
        EXPECT_NE(nullptr, data);
        return;
    }

    EXPECT_FALSE(is_const(*data));
    EXPECT_EQ(1, data[0]);
}

TYPED_TEST(Array_Member, Data_Const)
{
    const auto array = TypeParam::make_array();
    auto data = array.data();

    if (TypeParam::is_empty) {
        EXPECT_NE(nullptr, data);
        return;
    }

    EXPECT_TRUE(is_const(*data));
    EXPECT_EQ(1, data[0]);
}

TYPED_TEST(Array_Member, Begin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.begin()));
}

TYPED_TEST(Array_Member, Begin_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.begin()));
}

TYPED_TEST(Array_Member, CBegin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.cbegin()));
}

TYPED_TEST(Array_Member, Begin)
{
    auto array = TypeParam::make_array();
    auto begin = array.begin();

    EXPECT_FALSE(is_const(*begin));

    if (TypeParam::is_empty) return;

    EXPECT_EQ(1, *begin);
}

TYPED_TEST(Array_Member, Begin_Const)
{
    const auto array = TypeParam::make_array();
    auto begin = array.begin();

    EXPECT_TRUE(is_const(*begin));

    if (TypeParam::is_empty) return;

    EXPECT_EQ(1, *begin);
}

TYPED_TEST(Array_Member, CBegin)
{
    auto array = TypeParam::make_array();
    auto cbegin = array.cbegin();

    EXPECT_TRUE(is_const(*cbegin));

    if (TypeParam::is_empty) return;

    EXPECT_EQ(1, *cbegin);
}

TYPED_TEST(Array_Member, End_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.end()));
}

TYPED_TEST(Array_Member, End_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.end()));
}

TYPED_TEST(Array_Member, CEnd_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.cend()));
}

TYPED_TEST(Array_Member, End)
{
    auto array = TypeParam::make_array();
    auto end = array.end();

    EXPECT_FALSE(is_const(*end));
}

TYPED_TEST(Array_Member, End_Const)
{
    const auto array = TypeParam::make_array();
    auto end = array.end();

    EXPECT_TRUE(is_const(*end));
}

TYPED_TEST(Array_Member, CEnd)
{
    auto array = TypeParam::make_array();
    auto cend = array.cend();

    EXPECT_TRUE(is_const(*cend));
}

TYPED_TEST(Array_Member, Begin_End_Distance)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.begin(), array.end());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Member, Begin_End_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.begin(), array.end());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Member, CBegin_CEnd_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.cbegin(), array.cend());

    EXPECT_EQ(expected, distance);
}
