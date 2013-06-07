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

TEST(Relate, Default_True)
{
    bool value = relate<same>::template op<int, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Default_False)
{
    bool value = relate<same>::template op<int, double>::value;
    EXPECT_FALSE(value);
}
