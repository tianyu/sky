#include <type_traits>

#include "gtest/gtest.h"

#include "sky/array.hpp"

namespace {

template<typename T, std::size_t... Ns>
struct Param
{
    using value_type = T;
    using array_type = sky::array<T, Ns...>;
};

template<typename P>
struct Array_MemTypes : public ::testing::Test
{};

} // namespace

using MemTypes_Types = ::testing::Types<
    Param<int>,
    Param<int, 1>,
    Param<int, 2, 2>,
    Param<int, 3, 3, 3>
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

