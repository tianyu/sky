#include <gtest/gtest.h>
#include <type_traits>

#include <sky/atomic.hpp>

using namespace sky;

template<typename T>
class AtomicCounter : public testing::Test {};

typedef testing::Types<char, int, unsigned int, long> Integrals;

TYPED_TEST_CASE(AtomicCounter, Integrals);

TYPED_TEST(AtomicCounter, TypeTraits)
{
    using namespace std;
    typedef atomic_counter<TypeParam> counter_t;

    EXPECT_TRUE (is_nothrow_default_constructible<counter_t>::value);
    EXPECT_FALSE(is_copy_constructible<counter_t>::value);
    EXPECT_FALSE(is_move_constructible<counter_t>::value);
    EXPECT_FALSE(is_copy_assignable<counter_t>::value);
    EXPECT_FALSE(is_move_assignable<counter_t>::value);
    EXPECT_TRUE (is_destructible<counter_t>::value);
}

TYPED_TEST(AtomicCounter, ConstructDefault)
{
    atomic_counter<TypeParam>();
}

TYPED_TEST(AtomicCounter, ConstructInit)
{
    atomic_counter<TypeParam> t(23);

    EXPECT_EQ(23, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, ConstructInit_constexpr)
{
    static const atomic_counter<TypeParam> t(42);

    EXPECT_EQ(42, (TypeParam)const_cast<atomic_counter<TypeParam>&>(t));
}

TYPED_TEST(AtomicCounter, IsLockFree)
{
    std::atomic<TypeParam> variable;
    atomic_counter<TypeParam> counter;

    EXPECT_EQ(variable.is_lock_free(), counter.is_lock_free());
}

TYPED_TEST(AtomicCounter, IsLockFree_volatile)
{
    volatile std::atomic<TypeParam> variable;
    volatile atomic_counter<TypeParam> counter;

    EXPECT_EQ(variable.is_lock_free(), counter.is_lock_free());
}

TYPED_TEST(AtomicCounter, PreIncrement)
{
    atomic_counter<TypeParam> t(0);

    ++t;

    EXPECT_EQ(1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PreIncrement_volatile)
{
    volatile atomic_counter<TypeParam> t(0);

    ++t;

    EXPECT_EQ(1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PostIncrement)
{
    atomic_counter<TypeParam> t(0);

    t++;

    EXPECT_EQ(1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PostIncrement_volatile)
{
    volatile atomic_counter<TypeParam> t(0);

    t++;

    EXPECT_EQ(1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, IncrementValue)
{
    atomic_counter<TypeParam> t(0);

    t += 23;

    EXPECT_EQ(23, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, IncrementValue_volatile)
{
    volatile atomic_counter<TypeParam> t(0);

    t += 23;

    EXPECT_EQ(23, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PreDecrement)
{
    atomic_counter<TypeParam> t(0);

    --t;

    EXPECT_EQ(-1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PreDecrement_volatile)
{
    volatile atomic_counter<TypeParam> t(0);

    --t;

    EXPECT_EQ(-1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PostDecrement)
{
    atomic_counter<TypeParam> t(0);

    t--;

    EXPECT_EQ(-1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, PostDecrement_volatile)
{
    volatile atomic_counter<TypeParam> t(0);

    t--;

    EXPECT_EQ(-1, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, DecrementValue)
{
    atomic_counter<TypeParam> t(0);

    t -= 23;

    EXPECT_EQ(-23, (TypeParam)t);
}

TYPED_TEST(AtomicCounter, DecrementValue_volatile)
{
    volatile atomic_counter<TypeParam> t(0);

    t -= 23;

    EXPECT_EQ(-23, (TypeParam)t);
}
