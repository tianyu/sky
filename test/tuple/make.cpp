#include "gtest/gtest.h"

#include "../test_argument_passing.hpp"

#include "sky/tuple.hpp"

#include <string>
#include <utility>

using sky::make;

/* make() can return more than just value types */

TEST(Make, PointerType)
{
    int *p = make<int*>(std::make_tuple(nullptr));
    EXPECT_EQ(nullptr, p);
}

TEST(Make, ReferenceType)
{
    int i = 0;
    int &r = make<int&>(std::forward_as_tuple(i));

    i = 24;
    EXPECT_EQ(24, r);
}

/* make() can return value types and can accept multiple arguments */

TEST(Make, NoArguments)
{
    int i = make<int>(std::tuple<>());
    (void)i;
}

TEST(Make, OneArgument)
{
    int i = make<int>(std::make_tuple(1));
    EXPECT_EQ(1, i);
}

TEST(Make, TwoArguments)
{
    auto s = make<std::string>(std::make_tuple(5, 'a'));
    EXPECT_EQ("aaaaa", s);
}

TEST(Make, VariadicArguments)
{
    int i = 5;
    auto t = make< std::tuple<int, double, std::string, int&> >(
                std::forward_as_tuple(25, 3.14, "Meow", i));

    i = 42;

    EXPECT_EQ(25, std::get<0>(t));
    EXPECT_EQ(3.14, std::get<1>(t));
    EXPECT_EQ("Meow", std::get<2>(t));
    EXPECT_EQ(42, std::get<3>(t));
}

namespace {

struct TestClass
{
    TestClass(DataObject const&o) :
        data(o)
    {}

    TestClass(DataObject &&o) :
        data(std::move(o))
    {}

    DataObject data;
};

} // namespace

TEST(Make, CaptureBy_Lvalue)
{
    DataObject o;
    auto t = make<TestClass>(std::forward_as_tuple(o));

    EXPECT_EQ(1, t.data.copies);
    EXPECT_EQ(0, t.data.moves);
}

TEST(Make, CaptureBy_ConstLvalue)
{
    const DataObject o;
    auto t = make<TestClass>(std::forward_as_tuple(o));

    EXPECT_EQ(1, t.data.copies);
    EXPECT_EQ(0, t.data.moves);
}

TEST(Make, CaptureBy_Rvalue)
{
    auto t = make<TestClass>(std::forward_as_tuple(DataObject()));

    EXPECT_EQ(0, t.data.copies);
    EXPECT_EQ(1, t.data.moves);
}

TEST(Make, CaptureBy_ConstRvalue)
{
    const DataObject o;
    auto t = make<TestClass>(std::forward_as_tuple(std::move(o)));

    EXPECT_EQ(1, t.data.copies);
    EXPECT_EQ(0, t.data.moves);
}
