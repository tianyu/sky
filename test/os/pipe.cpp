#include "gtest/gtest.h"

#include "sky/os.h"

using namespace sky;

TEST(Pipe, Make)
{
    auto pipe = make_pipe();
}
