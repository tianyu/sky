#include <array>
#include <iterator>
#include <type_traits>
#include <utility>

#include "gtest/gtest.h"

#include "sky/type_list.hpp"

#include "sky/array.hpp"

namespace  {

template<std::size_t... Ns> struct Param
{
private:
    template<std::size_t... Vals>
    static std::array<int, sizeof...(Vals)>
    make_array_helper(sky::index_list<Vals...>)
    {
        return std::array<int, sizeof...(Vals)>{{ Vals... }};
    }

public:

    using array_t = sky::array<int, Ns...>;

    static const std::size_t size = sky::product<sky::index_list<Ns...>>::value;

    static const bool is_empty = (size == 0);

    static const int front = 1;

    static const int back = size;

    static int const* begin_of(array_t const&array)
    {
        return reinterpret_cast<int const*>(&array);
    }

    static int const* end_of(array_t const&array)
    {
        return begin_of(array) + size;
    }

    static int const* rbegin_of(array_t const&array)
    {
        return end_of(array) - 1;
    }

    static int const* rend_of(array_t const&array)
    {
        return begin_of(array) - 1;
    }

    static array_t make_array()
    {
        using values_t = typename sky::index_range<1, size + 1>::type;
        // This reinterpret cast should work since both sky::array and
        // std::array should have the same layout.
        return reinterpret_cast<array_t&&>(
                    std::move(make_array_helper(values_t())));
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
    auto expected = TypeParam::begin_of(array);
    auto data = array.data();

    if (TypeParam::is_empty) {
        EXPECT_NE(nullptr, data);
        return;
    }

    EXPECT_FALSE(is_const(*data));
    EXPECT_EQ(expected, data);
}

TYPED_TEST(Array_Member, Data_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto data = array.data();

    if (TypeParam::is_empty) {
        EXPECT_NE(nullptr, data);
        return;
    }

    EXPECT_TRUE(is_const(*data));
    EXPECT_EQ(expected, data);
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
    auto expected = TypeParam::begin_of(array);
    auto begin = array.begin();

    EXPECT_FALSE(is_const(*begin));
    EXPECT_EQ(expected, begin);
}

TYPED_TEST(Array_Member, Begin_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto begin = array.begin();

    EXPECT_TRUE(is_const(*begin));
    EXPECT_EQ(expected, begin);
}

TYPED_TEST(Array_Member, CBegin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto cbegin = array.cbegin();

    EXPECT_TRUE(is_const(*cbegin));
    EXPECT_EQ(expected, cbegin);
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
    auto expected = TypeParam::end_of(array);
    auto end = array.end();

    EXPECT_FALSE(is_const(*end));
    EXPECT_EQ(expected, end);
}

TYPED_TEST(Array_Member, End_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::end_of(array);
    auto end = array.end();

    EXPECT_TRUE(is_const(*end));
    EXPECT_EQ(expected, end);
}

TYPED_TEST(Array_Member, CEnd)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::end_of(array);
    auto cend = array.cend();

    EXPECT_TRUE(is_const(*cend));
    EXPECT_EQ(expected, cend);
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

TYPED_TEST(Array_Member, RBegin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rbegin()));
}

TYPED_TEST(Array_Member, RBegin_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rbegin()));
}

TYPED_TEST(Array_Member, CRBegin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.crbegin()));
}

TYPED_TEST(Array_Member, RBegin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rbegin_of(array);
    auto rbegin = array.rbegin();

    EXPECT_FALSE(is_const(*rbegin));
    EXPECT_EQ(expected, rbegin.base());
}

TYPED_TEST(Array_Member, RBegin_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::rbegin_of(array);
    auto rbegin = array.rbegin();

    EXPECT_TRUE(is_const(*rbegin));
    EXPECT_EQ(expected, rbegin.base());
}

TYPED_TEST(Array_Member, CRBegin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rbegin_of(array);
    auto crbegin = array.crbegin();

    EXPECT_TRUE(is_const(*crbegin));
    EXPECT_EQ(expected, crbegin.base());
}

TYPED_TEST(Array_Member, REnd_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rend()));
}

TYPED_TEST(Array_Member, REnd_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rend()));
}

TYPED_TEST(Array_Member, CREnd_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.crend()));
}

TYPED_TEST(Array_Member, REnd)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rend_of(array);
    auto rend = array.rend();

    EXPECT_FALSE(is_const(*rend));
    EXPECT_EQ(expected, rend.base());
}

TYPED_TEST(Array_Member, REnd_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::rend_of(array);
    auto rend = array.rend();

    EXPECT_TRUE(is_const(*rend));
    EXPECT_EQ(expected, rend.base());
}

TYPED_TEST(Array_Member, CREnd)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rend_of(array);
    auto crend = array.crend();

    EXPECT_TRUE(is_const(*crend));
    EXPECT_EQ(expected, crend.base());
}

TYPED_TEST(Array_Member, RBegin_REnd_Distance)
{
    auto array = TypeParam::make_array();
    auto expected = -TypeParam::size;
    auto distance = std::distance(array.rbegin(), array.rend());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Member, RBegin_REnd_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = -TypeParam::size;
    auto distance = std::distance(array.rbegin(), array.rend());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Member, CRBegin_CREnd_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = -TypeParam::size;
    auto distance = std::distance(array.crbegin(), array.crend());

    EXPECT_EQ(expected, distance);
}
