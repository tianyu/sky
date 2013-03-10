#include "gtest/gtest.h"
#include "../interface_tests.hpp"

#include "sky/semaphore.h"

// Interface tests

TEST(Semaphore, Interface)
{
    typedef InterfaceOf<sky::semaphore> ISemaphore;

    ISemaphore::expect_default_constructible();
    ISemaphore::expect_constructible<unsigned int>();

    // Semaphores should not be move/copy constructible/assignable
    ISemaphore::expect_copy_constructible(false);
    ISemaphore::expect_copy_assignable(false);
    ISemaphore::expect_move_constructible(false);
    ISemaphore::expect_move_assignable(false);
}
