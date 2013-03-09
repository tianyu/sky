#include "gtest/gtest.h"

#include "sky/concurrent_queue.hpp"

using sky::concurrent_queue;

TEST(ConcurrentQueue, Construct)
{
    concurrent_queue<int>();
}
