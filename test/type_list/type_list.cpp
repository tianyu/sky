#include "gtest/gtest.h"

#include "sky/type_list.hpp"

using sky::type_list;

TEST(TypeList, ConstructEmpty)
{
    type_list<int>();
}

TEST(TypeList, ConstructOne)
{
    type_list<int, 0>();
}

TEST(TypeList, ConstructMany)
{
    type_list<int, 0, 5, 3, 24>();
}
