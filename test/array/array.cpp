#include <array>
#include <iterator>
#include <type_traits>

#include "gtest/gtest.h"

#include "sky/type_list.hpp"

#include "sky/array.hpp"

namespace  {

template<std::size_t... Ns> struct Member_Param
{
private:
    template<std::size_t... Vals>
    static std::array<int, sizeof...(Vals)> &&
    make_array_helper(sky::index_list<Vals...>)
    {
        return std::array<int, sizeof...(Vals)>{{ Vals... }};
    }

public:
    static sky::array<int, Ns...> make_array()
    {
        enum { product = sky::product<sky::index_list<Ns...>>::value };
        using values_t = typename sky::index_range<1, product + 1>::type;
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
T const& to_const(T const&t) noexcept
{
    return t;
}

template<typename T>
constexpr bool is_const(T &) { return false; }

template<typename T>
constexpr bool is_const(T const&) { return true; }

} // namespace

using Member_Types = ::testing::Types<
    Member_Param<>,
    Member_Param<2>,
    Member_Param<2, 2>
>;

TYPED_TEST_CASE(Array_Member, Member_Types);

TYPED_TEST(Array_Member, Data_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.data()));
}

TYPED_TEST(Array_Member, Data_Const_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(to_const(array).data()));
}

TEST(Array_Member, Data_Empty)
{
    auto array = sky::array<int, 1, 0, 1>{};

    EXPECT_NE(nullptr, array.data());
}

TYPED_TEST(Array_Member, Data)
{
    auto array = TypeParam::make_array();
    auto data = array.data();

    EXPECT_FALSE(is_const(*data));
    EXPECT_EQ(1, data[0]);
}

TYPED_TEST(Array_Member, Data_Const)
{
    auto array = TypeParam::make_array();
    auto data = to_const(array).data();

    EXPECT_TRUE(is_const(*data));
    EXPECT_EQ(1, data[0]);
}

