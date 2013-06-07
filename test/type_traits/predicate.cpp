#include "gtest/gtest.h"

#include "sky/type_traits.hpp"

using namespace sky;

namespace {

struct same
{
    template<typename T, typename U>
    using op = std::is_same<T,U>;
};

} // namespace

TEST(PredicateAnd, Empty)
{
    bool value = predicate_and<same, int>::value;
    EXPECT_TRUE(value);
}

TEST(PredicateAnd, OneType_True)
{
    bool value = predicate_and<same, int, int>::value;
    EXPECT_TRUE(value);
}

TEST(PredicateAnd, OneType_False)
{
    bool value = predicate_and<same, int, double>::value;
    EXPECT_FALSE(value);
}

TEST(PredicateAnd, ManyTypes_True)
{
    bool value = predicate_and<same, int, int, int, int>::value;
    EXPECT_TRUE(value);
}

TEST(PredicateAnd, ManyTypes_False)
{
    bool value = predicate_and<same, int, int, double, int>::value;
    EXPECT_FALSE(value);
}

TEST(PredicateOr, Empty)
{
    bool value = predicate_or<same, int>::value;
    EXPECT_FALSE(value);
}

TEST(PredicateOr, OneType_True)
{
    bool value = predicate_or<same, int, int>::value;
    EXPECT_TRUE(value);
}

TEST(PredicateOr, OneType_False)
{
    bool value = predicate_or<same, int, double>::value;
    EXPECT_FALSE(value);
}

TEST(PredicateOr, ManyTypes_True)
{
    bool value = predicate_or<same, int, double, int, double>::value;
    EXPECT_TRUE(value);
}

TEST(PredicateOr, ManyTypes_False)
{
    bool value = predicate_or<same, int, double, double, double>::value;
    EXPECT_FALSE(value);
}
