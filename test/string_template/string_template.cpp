#include "gtest/gtest.h"

#include "sky/string_template.hpp"

static
void test(char const s[])
{

}

TEST(StringTemplate, Typedef)
{
    test("Hello");
    using namespace sky;
    MAKE_STR<"Hello">::type;
}
