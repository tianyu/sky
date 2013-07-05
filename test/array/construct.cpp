#include <array>

#include "gtest/gtest.h"

#include "sky/array.hpp"

TEST(Array_Construct, Dim0_Default)
{
    (void)sky::array<int>{};
}

TEST(Array_Construct, Dim0_Aggregate)
{
    (void)sky::array<int>{1};
}

TEST(Array_Construct, Dim1_Default)
{
    (void)sky::array<int, 2>{};
}

TEST(Array_Construct, Dim1_Aggregate)
{
    (void)sky::array<int, 2>{{1, 2}};
}

TEST(Array_Construct, Dim2_Default)
{
    (void)sky::array<int, 2, 2>{};
}

TEST(Array_Construct, Dim2_Aggregate)
{
    (void)sky::array<int, 2, 2>{{
        {{2, 4}},
        {{6, 8}}
    }};
}

TEST(Array_Construct, Dim3_Default)
{
    (void)sky::array<int, 2, 3, 2>{};
}

TEST(Array_Construct, Dim3_Aggregate)
{
    (void)sky::array<int, 2, 3, 2>{{
        {{
            {{1, 2}},
            {{3, 4}},
            {{5, 6}}
        }},{{
            {{7, 8}},
            {{9, 10}},
            {{11, 12}}
        }}
    }};
}
