#include <array>
#include <iterator>
#include <type_traits>

#include "gtest/gtest.h"

#include "sky/type_list.hpp"

#include "sky/array.hpp"

// Member Type tests //

namespace {

template<typename T, std::size_t... Ns>
struct MemType_Param
{
    using value_type = T;
    using array_type = sky::array<T, Ns...>;
};

template<typename P>
struct Array_MemTypes : public ::testing::Test
{};

} // namespace

using MemTypes_Types = ::testing::Types<
    MemType_Param<int>,
    MemType_Param<int, 1>,
    MemType_Param<int, 2, 2>,
    MemType_Param<int, 3, 3, 3>
>;

TYPED_TEST_CASE(Array_MemTypes, MemTypes_Types);

TYPED_TEST(Array_MemTypes, Same_Value_Type)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_value_type = std::is_same<
            typename expected_t::value_type,
            typename array_t::value_type
        >::value;
    EXPECT_TRUE(same_value_type);
}

TYPED_TEST(Array_MemTypes, Same_Size_Type)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_size_type = std::is_same<
            typename expected_t::size_type,
            typename array_t::size_type
        >::value;
    EXPECT_TRUE(same_size_type);
}

TYPED_TEST(Array_MemTypes, Same_Difference_Type)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_difference_type = std::is_same<
            typename expected_t::difference_type,
            typename array_t::difference_type
        >::value;
    EXPECT_TRUE(same_difference_type);
}

TYPED_TEST(Array_MemTypes, Same_Reference)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_reference = std::is_same<
            typename expected_t::reference,
            typename array_t::reference
        >::value;
    EXPECT_TRUE(same_reference);
}

TYPED_TEST(Array_MemTypes, Same_Const_Reference)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_const_reference = std::is_same<
            typename expected_t::const_reference,
            typename array_t::const_reference
        >::value;
    EXPECT_TRUE(same_const_reference);
}

TYPED_TEST(Array_MemTypes, Same_Pointer)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_pointer = std::is_same<
            typename expected_t::pointer,
            typename array_t::pointer
        >::value;
    EXPECT_TRUE(same_pointer);
}

TYPED_TEST(Array_MemTypes, Same_Const_Pointer)
{
    using expected_t = std::array<typename TypeParam::value_type,1>;
    using array_t = typename TypeParam::array_type;

    const auto same_const_pointer = std::is_same<
            typename expected_t::const_pointer,
            typename array_t::const_pointer
        >::value;
    EXPECT_TRUE(same_const_pointer);
}

TYPED_TEST(Array_MemTypes, RandomAccess_Iterator)
{
    using array_t = typename TypeParam::array_type;

    const auto random_access_iterator = std::is_same<
            std::random_access_iterator_tag,
            typename std::iterator_traits<
                typename array_t::iterator
            >::iterator_category
        >::value;
    EXPECT_TRUE(random_access_iterator);
}

TYPED_TEST(Array_MemTypes, RandomAccess_Const_Iterator)
{
    using array_t = typename TypeParam::array_type;

    const auto random_access_iterator = std::is_same<
            std::random_access_iterator_tag,
            typename std::iterator_traits<
                typename array_t::const_iterator
            >::iterator_category
        >::value;
    EXPECT_TRUE(random_access_iterator);

    const auto is_const_iterator = std::is_const<
            typename std::remove_reference<
                decltype(*std::declval<
                    typename array_t::const_iterator>())
            >::type
        >::value;
    EXPECT_TRUE(is_const_iterator);
}

TYPED_TEST(Array_MemTypes, RandomAccess_Reverse_Iterator)
{
    using array_t = typename TypeParam::array_type;

    const auto random_access_reverse_iterator = std::is_same<
            std::random_access_iterator_tag,
            typename std::iterator_traits<
                typename array_t::reverse_iterator
            >::iterator_category
        >::value;
    EXPECT_TRUE(random_access_reverse_iterator);
}

TYPED_TEST(Array_MemTypes, RandomAccess_Const_Reverse_Iterator)
{
    using array_t = typename TypeParam::array_type;

    const auto random_access_reverse_iterator = std::is_same<
            std::random_access_iterator_tag,
            typename std::iterator_traits<
                typename array_t::const_reverse_iterator
            >::iterator_category
        >::value;
    EXPECT_TRUE(random_access_reverse_iterator);

    const auto is_const_reverse_iterator = std::is_const<
            typename std::remove_reference<
                decltype(*std::declval<
                    typename array_t::const_reverse_iterator>())
            >::type
        >::value;
    EXPECT_TRUE(is_const_reverse_iterator);
}

// Constructor Tests //

TEST(Array, Construct_Dim0_Default)
{
    (void)sky::array<int>{};
}

TEST(Array, Construct_Dim0_Aggregate)
{
    (void)sky::array<int>{1};
}

TEST(Array, Construct_Dim1_Default)
{
    (void)sky::array<int, 2>{};
}

TEST(Array, Construct_Dim1_Aggregate)
{
    (void)sky::array<int, 2>{{1, 2}};
}

TEST(Array, Construct_Dim2_Default)
{
    (void)sky::array<int, 2, 2>{};
}

TEST(Array, Construct_Dim2_Aggregate)
{
    (void)sky::array<int, 2, 2>{{
        {{2, 4}},
        {{6, 8}}
    }};
}

TEST(Array, Construct_Dim3_Default)
{
    (void)sky::array<int, 2, 3, 2>{};
}

TEST(Array, Construct_Dim3_Aggregate)
{
    (void)sky::array<int, 2, 3, 2>{{
        {{
            {{1, 2}},
            {{3, 4}},
            {{5, 6}}
        }},{{
            {{7, 8}},
            {{9, 10}},
            {{11, 12}}
        }}
    }};
}

// Member Tests //

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

TEST(Array, Data_Empty)
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

