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
