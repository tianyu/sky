#include "gtest/gtest.h"

#include "common.hpp"

TEST(Array_Operation, Fill)
{
    auto array = sky::array<int>{};
    array.fill(3);

    for (auto &value : array) {
        EXPECT_EQ(3, value);
    }
}
