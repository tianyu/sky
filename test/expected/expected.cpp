#include "sky/expected.hpp"
#include "gtest/gtest.h"

using sky::error;
using sky::expected;

namespace {

template<typename T, typename E>
void expect_error(expected<T, void> value, E error);

template<typename T, typename E>
void expect_error(expected<T, E> value, E error);

}

TEST(Expected, ConstructWithType)
{
    expected<int> x(0);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int)x);
    EXPECT_EQ(0, (int)(const expected<int>&)x);
}

TEST(ExpectedOrE, ConstructWithType)
{
    typedef expected<int, int> expected_t;
    expected_t x(0);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int)x);
    EXPECT_EQ(0, (int)(const expected_t&)x);
}

TEST(Expected, ConstructWithPointer)
{
    expected<int*> x(nullptr);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(nullptr, (int*)x);
    EXPECT_EQ(nullptr, (int*)(const expected<int*>&)x);
}

TEST(ExpectedOrE, ConstructWithPointer)
{
    typedef expected<int *, int> expected_t;
    expected_t x(nullptr);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(nullptr, (int*)x);
    EXPECT_EQ(nullptr, (int*)(const expected_t&)x);
}

TEST(Expected, ConstructWithReference)
{
    int i = 0;
    expected<int&> x(i);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int&)x);
    EXPECT_EQ(0, (int&)(const expected<int&>&)x);
}

TEST(ExpectedOrE, ConstructWithReference)
{
    int i = 0;
    typedef expected<int&, int> expected_t;
    expected_t x(i);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int&)x);
    EXPECT_EQ(0, (int&)(const expected_t&)x);
}

TEST(Expected, ConstructWithError)
{
    expected<int> x(error(5));
    EXPECT_FALSE(x.valid());
    expect_error(x, 5);
}

TEST(ExpectedOrE, ConstructWithError)
{
    typedef expected<int, int> expected_t;
    expected_t x(error(5));
    EXPECT_FALSE(x.valid());
    expect_error(x, 5);
}

TEST(Expected, ConstructWithCurrentException)
{
    try {
        throw 5;

    } catch (...) {
        // We shouldn't depend on the copy/move constructor here
        // Fix after uniform initialization works.
        expected<int> x = error();
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

TEST(ExpectedOrE, CopyValid)
{
    typedef expected<int, int> expected_t;
    const expected_t x(5);
    expected_t y(x);

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

TEST(ExpectedOrE, CopyInvalid)
{
    typedef expected<int, int> expected_t;
    const expected_t x(error(5));
    expected_t y(x);

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

TEST(ExpectedOrE, MoveValid)
{
    typedef expected<int, int> expected_t;
    expected_t x(3);
    expected_t y(std::move(x));

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

TEST(ExpectedOrE, MoveInvalid)
{
    typedef expected<int, int> expected_t;
    expected_t x(error(5));
    expected_t y(std::move(x));

    EXPECT_FALSE(y.valid());
    expect_error(y, 5);
}

TEST(ExpectedOrE, ErrorValid)
{
    typedef expected<int, int> expected_t;
    expected_t x(5);

    EXPECT_THROW(x.error(), std::logic_error);
    EXPECT_THROW(((const expected_t&)x).error(), std::logic_error);
}

TEST(ExpectedOrE, ErrorInvalid)
{
    typedef expected<int, int> expected_t;
    expected_t x(error(5));

    EXPECT_EQ(5, x.error());
    EXPECT_EQ(5, ((const expected_t&)x).error());
}

namespace {

template<typename Cast, typename T, typename E, typename X>
void try_expect_error(expected<T, E> value, X error)
try {
    (void)(T)(Cast)value;
    FAIL()
            << "Expected an exception of type \""
            << typeid(X).name() << "\".";
} catch (X &e) {
    EXPECT_EQ(error, e);
} catch (...) {
    FAIL()
            << "Unexpected exception! "
               "Expected exception of type \""
            << typeid(X).name() << "\".";
}

template<typename T, typename E>
void expect_error(expected<T, void> value, E error)
{
    try_expect_error<expected<T> &>(value, error);
    try_expect_error<expected<T> const&>(value, error);
}

template<typename T, typename E>
void expect_error(expected<T, E> value, E error)
{
    try_expect_error<expected<T,E> &>(value, error);
    try_expect_error<expected<T,E> const&>(value, error);
}

} // namespace
