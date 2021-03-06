#include "gtest/gtest.h"
#include "../interface_tests.hpp"

#include "sky/semaphore.h"

// Interface tests

TEST(Semaphore, Interface)
{
    typedef InterfaceOf<sky::semaphore> ISemaphore;

    ISemaphore::expect_default_constructible();
    ISemaphore::expect_constructible<char>();
    ISemaphore::expect_constructible<unsigned short>();
    ISemaphore::expect_constructible<int>();
    ISemaphore::expect_constructible<std::size_t>(false);
    ISemaphore::expect_constructible<unsigned int>(false);
    ISemaphore::expect_constructible<unsigned long>(false);

    // Semaphores should not be move/copy constructible/assignable
    ISemaphore::expect_copy_constructible(false);
    ISemaphore::expect_copy_assignable(false);
    ISemaphore::expect_move_constructible(false);
    ISemaphore::expect_move_assignable(false);
}

TEST(Semaphore, UsableWithLockGuard)
{
    sky::semaphore s;
    std::lock_guard<sky::semaphore> lock(s);
}

TEST(Semaphore, UsableWithUniqueLock)
{
    sky::semaphore s;
    std::unique_lock<sky::semaphore> lock(s, std::defer_lock_t());

    EXPECT_TRUE(lock.try_lock());
}

TEST(Semaphore, SizeOf)
{
    size_t expected
            = sizeof(std::mutex)
            + sizeof(std::condition_variable)
            + 2*sizeof(int);

    EXPECT_EQ(expected, sizeof(sky::semaphore));
}

// Unit Tests

using sky::semaphore;

TEST(Semaphore, DefaultConstruct)
{
    semaphore s;

    EXPECT_TRUE(s.try_acquire());
    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, TryP)
{
    semaphore s;

    EXPECT_TRUE(s.try_P());
    EXPECT_FALSE(s.try_P());
}

TEST(Semaphore, ConstructWithNoResources)
{
    semaphore s(0);

    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, ConstructWithNegativeResources)
{
    semaphore s(-1);

    EXPECT_FALSE(s.try_acquire());
    s.release();
    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, ConstructWithPositiveResources)
{
    semaphore s(20);

    for (unsigned i = 0; i < 20u; ++i) {
        EXPECT_TRUE(s.try_acquire());
    }
    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, Acquire)
{
    semaphore s(1);

    s.acquire();

    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, P)
{
    semaphore s(1);

    s.P();

    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, Release)
{
    semaphore s(0);

    EXPECT_FALSE(s.try_acquire());

    s.release();

    EXPECT_TRUE(s.try_acquire());
}

TEST(Semaphore, V)
{
    semaphore s(0);

    EXPECT_FALSE(s.try_acquire());

    s.release();

    EXPECT_TRUE(s.try_acquire());
}
