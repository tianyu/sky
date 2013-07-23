#include <array>

#include "gtest/gtest.h"

#include "sky/type_list.hpp"

#include "sky/array.hpp"

namespace  {

template<std::size_t... Ns>
void expect_default_constructed(sky::array<int, Ns...> const&array)
{
    enum { size = sky::product<sky::index_list<Ns...>>::value };
    auto data = reinterpret_cast<int const*>(&array);
    for (int i = 1; i <= size; ++i) {
        EXPECT_EQ(0, *data);
        data++;
    }
}

template<std::size_t... Ns>
void expect_sequence_constructed(sky::array<int, Ns...> const&array)
{
    enum { size = sky::product<sky::index_list<Ns...>>::value };
    auto data = reinterpret_cast<int const*>(&array);
    for (int expected = 1; expected <= size; ++expected) {
        EXPECT_EQ(expected, *data);
        data++;
    }
}

} // namespace

TEST(Array_Construct, Dim0_Size)
{
    EXPECT_EQ(sizeof(int), sizeof(sky::array<int>));
}

TEST(Array_Construct, Dim0_Default)
{
    auto array = sky::array<int>{};
    expect_default_constructed(array);
}

TEST(Array_Construct, Dim0_Aggregate)
{
    auto array1 = sky::array<int>{1};
    expect_sequence_constructed(array1);

    auto array2 = sky::array<int>{{}};
    expect_default_constructed(array2);
}

TEST(Array_Construct, Dim1_Size)
{
    EXPECT_EQ(2*sizeof(int), sizeof(sky::array<int,2>));
    EXPECT_EQ(0, sizeof(sky::array<int,0>));
}

TEST(Array_Construct, Dim1_Default)
{
    auto array = sky::array<int, 2>{};
    expect_default_constructed(array);
}

TEST(Array_Construct, Dim1_Aggregate)
{
    auto array1 = sky::array<int, 2>{{1, 2}};
    expect_sequence_constructed(array1);

    auto array2 = sky::array<int, 2>{{{}, {}}};
    expect_default_constructed(array2);
}

TEST(Array_Construct, Dim2_Size)
{
    EXPECT_EQ(4*sizeof(int), sizeof(sky::array<int, 2, 2>));
    EXPECT_EQ(0, sizeof(sky::array<int, 0, 2>));
    EXPECT_EQ(0, sizeof(sky::array<int, 2, 0>));
}

TEST(Array_Construct, Dim2_Default)
{
    auto array = sky::array<int, 2, 2>{};
    expect_default_constructed(array);
}

TEST(Array_Construct, Dim2_Aggregate)
{
    auto array1 = sky::array<int, 2, 2>{{
        {{1, 2}},
        {{3, 4}}
    }};
    expect_sequence_constructed(array1);

    auto array2 = sky::array<int, 2, 2>{{
        {{ {}, {} }},
        {{ {}, {} }}
    }};
    expect_default_constructed(array2);
}

TEST(Array_Construct, Dim3_Size)
{
    EXPECT_EQ(12*sizeof(int), sizeof(sky::array<int, 2, 3, 2>));
    EXPECT_EQ(0, sizeof(sky::array<int, 2, 3, 0>));
    EXPECT_EQ(0, sizeof(sky::array<int, 2, 0, 3>));
    EXPECT_EQ(0, sizeof(sky::array<int, 0, 3, 2>));
}

TEST(Array_Construct, Dim3_Default)
{
    auto array = sky::array<int, 2, 3, 2>{};
    expect_default_constructed(array);
}

TEST(Array_Construct, Dim3_Aggregate)
{
    auto array1 = sky::array<int, 2, 3, 2>{{
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
    expect_sequence_constructed(array1);

    auto array2 = sky::array<int, 2, 3, 2>{{
        {{
            {{ {}, {} }},
            {{ {}, {} }},
            {{ {}, {} }}
        }}, {{
            {{ {}, {} }},
            {{ {}, {} }},
            {{ {}, {} }}
        }}
    }};
    expect_default_constructed(array2);
}
