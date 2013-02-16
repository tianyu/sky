#include <gtest/gtest.h>

#include <sky/atomic.hpp>

using namespace sky;

template<typename T>
class AtomicCounter : public testing::Test {};

typedef testing::Types<char, int, unsigned int, long> Integrals;

TYPED_TEST_CASE(AtomicCounter, Integrals);

TYPED_TEST(AtomicCounter, ConstructDefault)
{
    atomic_counter<TypeParam> t;

    EXPECT_EQ(0, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, ConstructEmplace)
{
    atomic_counter<TypeParam> t(23);

    EXPECT_EQ(23, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PreIncrement)
{
    atomic_counter<TypeParam> t(0);

    ++t;

    EXPECT_EQ(1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PostIncrement)
{
    atomic_counter<TypeParam> t(0);

    t++;

    EXPECT_EQ(1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, IncrementValue)
{
    atomic_counter<TypeParam> t(0);

    t += 23;

    EXPECT_EQ(23, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PreDecrement)
{
    atomic_counter<TypeParam> t(0);

    --t;

    EXPECT_EQ(-1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PostDecrement)
{
    atomic_counter<TypeParam> t(0);

    t--;

    EXPECT_EQ(-1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, DecrementValue)
{
    atomic_counter<TypeParam> t(0);

    t -= 23;

    EXPECT_EQ(-23, (TypeParam)t);
}
