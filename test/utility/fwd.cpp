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

    bool convertible_to_T =
        std::is_convertible<sky::fwd<T>, T>::value;
    EXPECT_TRUE(convertible_to_T);
}

// Begin definitions for unit tests //

namespace {

struct Object
{
    enum State { Elided, Copied, Moved };

    Object(int data) : data(data), state(Elided) {}

    Object(Object const&o) : data(o.data), state(Copied) {}

    Object(Object &&o) : data(o.data), state(Moved) {}

    int data;
    State state;
};

} // namespace

// Begin unit tests //

using namespace sky;

TEST(Fwd, CaptureLvalue)
{
    Object original(2352);
    fwd<Object> f(original);
    Object duplicate(f);

    EXPECT_EQ(Object::Copied, duplicate.state);
    EXPECT_EQ(2352, duplicate.data);
}

TEST(Fwd, CaptureConstLvalue)
{
    const Object original(4491);
    fwd<Object> f(original);
    Object duplicate(f);

    EXPECT_EQ(Object::Copied, duplicate.state);
    EXPECT_EQ(4491, duplicate.data);
}

TEST(Fwd, CaptureRvalue)
{
    Object original(4759);
    fwd<Object> f(std::move(original));
    Object duplicate(f);

    EXPECT_EQ(Object::Moved, duplicate.state);
    EXPECT_EQ(4759, duplicate.data);
}

TEST(Fwd, CaptureConstRvalue)
{
    const Object original(1030);
    fwd<Object> f(std::move(original));
    Object duplicate(f);

    EXPECT_EQ(Object::Copied, duplicate.state);
    EXPECT_EQ(1030, duplicate.data);
}
