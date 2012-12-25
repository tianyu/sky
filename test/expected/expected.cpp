#include "sky/expected.hpp"
#include "gtest/gtest.h"

using sky::error;
using sky::expected;

namespace {

template<typename T, typename E>
void expect_error(expected<T> value, E error);

}

TEST(Expected, ConstructWithType)
{
    expected<int> x(0);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int)x);
    EXPECT_EQ(0, (int)(const expected<int>&)x);
}

TEST(Expected, ConstructWithPointer)
{
    expected<int*> x(nullptr);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(nullptr, (int*)x);
    EXPECT_EQ(nullptr, (int*)(const expected<int*>&)x);
}

TEST(Expected, ConstructWithReference)
{
    int i = 0;
    expected<int&> x(i);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int&)x);
    EXPECT_EQ(0, (int)(const expected<int&>&)x);
}

TEST(Expected, ConstructWithError)
{
    expected<int> x(error(5));
    EXPECT_FALSE(x.valid());
    expect_error(x, 5);
}

TEST(Expected, ConstructWithCurrentException)
{
    try {
        throw 5;

    } catch (...) {
        expected<int> x = error(); // Fix after uniform initialization works
        EXPECT_FALSE(x.valid());
        expect_error(x, 5);
    }
}

TEST(Expected, CopyValid)
{
    const expected<int> x(5);
    expected<int> y(x);

    EXPECT_TRUE(x.valid());
    EXPECT_EQ(5,x);
    EXPECT_TRUE(y.valid());
    EXPECT_EQ(5,y);
}

TEST(Expected, CopyInvalid)
{
    const expected<int> x(error(5));
    expected<int> y(x);

    EXPECT_FALSE(x.valid());
    expect_error(x, 5);
    EXPECT_FALSE(y.valid());
    expect_error(y, 5);
}

TEST(Expected, MoveValid)
{
    expected<int> x(3);
    expected<int> y(std::move(x));

    EXPECT_TRUE(y.valid());
    EXPECT_EQ(3,y);
}

TEST(Expected, MoveInvalid)
{
    expected<int> x(error(5));
    expected<int> y(std::move(x));

    EXPECT_FALSE(y.valid());
    expect_error(y, 5);
}

namespace {

template<typename T, typename E, typename Cast = expected<T>&>
void try_expect_error(expected<T> value, E error)
try {
    (void)(T)(Cast)value;
    FAIL()
            << "Expected an exception of type \""
            << typeid(E).name() << "\".";
} catch (E &e) {
    EXPECT_EQ(error, e);
} catch (...) {
    FAIL()
            << "Unexpected exception! "
               "Expected exception of type \""
            << typeid(E).name() << "\".";
}

template<typename T, typename E>
void expect_error(expected<T> value, E error)
{
    try_expect_error<T,E>(value, error);
    try_expect_error<T,E,expected<T> const&>(value, error);
}

}
