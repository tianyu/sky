#include "gtest/gtest.h"

#include <type_traits>

#include "sky/utility.hpp"

// Interface tests //


TEST(Fwd, Constructible)
{
    typedef int T;

    EXPECT_FALSE(std::is_default_constructible<sky::fwd<T>>::value)
            << "fwd<T> should not be default constructible. "
               "It must capture a reference.";

    EXPECT_FALSE(std::is_copy_constructible<sky::fwd<T>>::value)
            << "fwd<T> should not be copy constructible, in case multiple "
               "levels of forwarding are required, std::fwd<T> objects "
               "should be moved instead. This is because the fwd may have "
               "captured an r-value reference, which requires unique "
               "ownership.";

    EXPECT_FALSE(std::is_copy_assignable<sky::fwd<T>>::value)
            << "fwd<T> should be not copy assignable. "
               "Move construction should be used instead.";

    EXPECT_TRUE(std::is_move_constructible<sky::fwd<T>>::value)
            << "fwd<T> should be move constructible in case multiple levels of"
               "forwarding are required.";

    EXPECT_FALSE(std::is_move_assignable<sky::fwd<T>>::value)
            << "fwd<T> should not be move assignable. "
               "Move construction should be used instead.";

    bool value_constructible =
        std::is_constructible<sky::fwd<T>, T>::value;
    EXPECT_TRUE(value_constructible);

    bool lvalue_constructible =
        std::is_constructible<sky::fwd<T>, T &>::value;
    EXPECT_TRUE(lvalue_constructible);

    bool const_lvalue_constructible =
        std::is_constructible<sky::fwd<T>, T const&>::value;
    EXPECT_TRUE(const_lvalue_constructible);

    bool rvalue_constructible =
        std::is_constructible<sky::fwd<T>, T &&>::value;
    EXPECT_TRUE(rvalue_constructible);

    bool const_rvalue_constructible =
        std::is_constructible<sky::fwd<T>, T const&&>::value;
    EXPECT_TRUE(const_rvalue_constructible);

    bool volatile_value_constructible =
        std::is_constructible<sky::fwd<volatile T>, volatile T>::value;
    EXPECT_TRUE(volatile_value_constructible);

    bool volatile_lvalue_constructible =
        std::is_constructible<sky::fwd<volatile T>, volatile T &>::value;
    EXPECT_TRUE(volatile_lvalue_constructible);

    bool volatile_const_lvalue_constructible =
        std::is_constructible<sky::fwd<volatile T>, volatile T const&>::value;
    EXPECT_TRUE(volatile_const_lvalue_constructible);

    bool volatile_rvalue_constructible =
        std::is_constructible<sky::fwd<volatile T>, volatile T &&>::value;
    EXPECT_TRUE(volatile_rvalue_constructible);

    bool volatile_const_rvalue_constructible =
        std::is_constructible<sky::fwd<volatile T>, volatile T const&&>::value;
    EXPECT_FALSE(volatile_const_rvalue_constructible);

    bool convertible_to_T =
        std::is_convertible<sky::fwd<T>, T>::value;
    EXPECT_TRUE(convertible_to_T);

    bool convertible_to_volatile_T =
        std::is_convertible<sky::fwd<volatile T>, volatile T>::value;
    EXPECT_TRUE(convertible_to_volatile_T);
}
