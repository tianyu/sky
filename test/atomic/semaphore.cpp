#include "gtest/gtest.h"
#include "../interface_tests.hpp"

#include "sky/semaphore.h"

// Interface tests

TEST(Semaphore, Interface)
{
    typedef InterfaceOf<sky::semaphore> ISemaphore;

    ISemaphore::expect_default_constructible();
    ISemaphore::expect_constructible<std::size_t>();
    ISemaphore::expect_constructible<unsigned int>();
    ISemaphore::expect_constructible<int>(false);
    ISemaphore::expect_constructible<unsigned long>(false);

    // Semaphores should not be move/copy constructible/assignable
    ISemaphore::expect_copy_constructible(false);
    ISemaphore::expect_copy_assignable(false);
    ISemaphore::expect_move_constructible(false);
    ISemaphore::expect_move_assignable(false);
}

// Unit Tests

using sky::semaphore;

TEST(Semaphore, DefaultConstruct)
{
    semaphore s;

    EXPECT_TRUE(s.try_acquire());
    EXPECT_FALSE(s.try_acquire());
}

TEST(Semaphore, ConstructWithResources)
{
    semaphore s(20u);

    for (unsigned i = 0; i < 20u; ++i) {
        EXPECT_TRUE(s.try_acquire());
    }
    EXPECT_FALSE(s.try_acquire());
}
