#include <type_traits>

#include "gtest/gtest.h"

#include "sky/array.hpp"

using sky::array;

TEST(Array, Same_Value_Type)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_value_type = std::is_same<
            expected_t::value_type,
            actual_t::value_type
        >::value;
    EXPECT_TRUE(same_value_type);
}

TEST(Array, Same_Size_Type)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_size_type = std::is_same<
            expected_t::size_type,
            actual_t::size_type
        >::value;
    EXPECT_TRUE(same_size_type);
}

TEST(Array, Same_Difference_Type)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_difference_type = std::is_same<
            expected_t::difference_type,
            actual_t::difference_type
        >::value;
    EXPECT_TRUE(same_difference_type);
}

TEST(Array, Same_Reference)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_reference = std::is_same<
            expected_t::reference,
            actual_t::reference
        >::value;
    EXPECT_TRUE(same_reference);
}

TEST(Array, Same_Const_Reference)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_const_reference = std::is_same<
            expected_t::const_reference,
            actual_t::const_reference
        >::value;
    EXPECT_TRUE(same_const_reference);
}

TEST(Array, Same_Pointer)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_pointer = std::is_same<
            expected_t::pointer,
            actual_t::pointer
        >::value;
    EXPECT_TRUE(same_pointer);
}

TEST(Array, Same_Const_Pointer)
{
    using expected_t = std::array<int,0>;
    using actual_t = array<int,0>;

    const auto same_const_pointer = std::is_same<
            expected_t::const_pointer,
            actual_t::const_pointer
        >::value;
    EXPECT_TRUE(same_const_pointer);
}

TEST(Array, RandomAccess_Iterator)
{
    using array_t = array<int, 0>;

    const auto random_access_iterator = std::is_same<
            std::random_access_iterator_tag,
            std::iterator_traits<array_t::iterator>::iterator_category
        >::value;
    EXPECT_TRUE(random_access_iterator);
}

TEST(Array, RandomAccess_Const_Iterator)
{
    using array_t = array<int, 0>;

    const auto random_access_iterator = std::is_same<
            std::random_access_iterator_tag,
            std::iterator_traits<array_t::const_iterator>::iterator_category
        >::value;
    EXPECT_TRUE(random_access_iterator);

    const auto is_const_iterator = std::is_const<
            std::remove_reference<
                decltype(*std::declval<array_t::const_iterator>())
            >::type
        >::value;
    EXPECT_TRUE(is_const_iterator);
}

TEST(Array, RandomAccess_Reverse_Iterator)
{
    using array_t = array<int, 0>;

    const auto random_access_reverse_iterator = std::is_same<
            std::random_access_iterator_tag,
            std::iterator_traits<array_t::reverse_iterator>::iterator_category
        >::value;
    EXPECT_TRUE(random_access_reverse_iterator);
}

TEST(Array, RandomAccess_Const_Reverse_Iterator)
{
    using array_t = array<int, 0>;

    const auto random_access_reverse_iterator = std::is_same<
            std::random_access_iterator_tag,
            std::iterator_traits<array_t::const_reverse_iterator>::iterator_category
        >::value;
    EXPECT_TRUE(random_access_reverse_iterator);

    const auto is_const_reverse_iterator = std::is_const<
            std::remove_reference<
                decltype(*std::declval<array_t::const_reverse_iterator>())
            >::type
        >::value;
    EXPECT_TRUE(is_const_reverse_iterator);
}

