#include "gtest/gtest.h"

#include "common.hpp"

template<typename P>
struct Array_Iterator : public ::testing::Test
{};

using Types = ::testing::Types<
    IntParam<>,
    IntParam<2>,
    IntParam<2, 2>,
    IntParam<0>,
    IntParam<2, 0>
>;

TYPED_TEST_CASE(Array_Iterator, Types);

TYPED_TEST(Array_Iterator, Data_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.data()));
}

TYPED_TEST(Array_Iterator, Data_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.data()));
}

TYPED_TEST(Array_Iterator, Data)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto data = array.data();

    EXPECT_NE(nullptr, data);
    EXPECT_FALSE(is_const(*data));
    EXPECT_EQ(expected, data);
}

TYPED_TEST(Array_Iterator, Data_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto data = array.data();

    EXPECT_NE(nullptr, data);
    EXPECT_TRUE(is_const(*data));
    EXPECT_EQ(expected, data);
}

TYPED_TEST(Array_Iterator, Begin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.begin()));
}

TYPED_TEST(Array_Iterator, Begin_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.begin()));
}

TYPED_TEST(Array_Iterator, CBegin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.cbegin()));
}

TYPED_TEST(Array_Iterator, Begin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto begin = array.begin();

    EXPECT_FALSE(is_const(*begin));
    EXPECT_EQ(expected, begin);
}

TYPED_TEST(Array_Iterator, Begin_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto begin = array.begin();

    EXPECT_TRUE(is_const(*begin));
    EXPECT_EQ(expected, begin);
}

TYPED_TEST(Array_Iterator, CBegin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::begin_of(array);
    auto cbegin = array.cbegin();

    EXPECT_TRUE(is_const(*cbegin));
    EXPECT_EQ(expected, cbegin);
}

TYPED_TEST(Array_Iterator, End_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.end()));
}

TYPED_TEST(Array_Iterator, End_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.end()));
}

TYPED_TEST(Array_Iterator, CEnd_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.cend()));
}

TYPED_TEST(Array_Iterator, End)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::end_of(array);
    auto end = array.end();

    EXPECT_FALSE(is_const(*end));
    EXPECT_EQ(expected, end);
}

TYPED_TEST(Array_Iterator, End_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::end_of(array);
    auto end = array.end();

    EXPECT_TRUE(is_const(*end));
    EXPECT_EQ(expected, end);
}

TYPED_TEST(Array_Iterator, CEnd)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::end_of(array);
    auto cend = array.cend();

    EXPECT_TRUE(is_const(*cend));
    EXPECT_EQ(expected, cend);
}

TYPED_TEST(Array_Iterator, Begin_End_Distance)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.begin(), array.end());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Iterator, Begin_End_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.begin(), array.end());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Iterator, CBegin_CEnd_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.cbegin(), array.cend());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Iterator, RBegin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rbegin()));
}

TYPED_TEST(Array_Iterator, RBegin_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rbegin()));
}

TYPED_TEST(Array_Iterator, CRBegin_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.crbegin()));
}

TYPED_TEST(Array_Iterator, RBegin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rbegin_of(array);
    auto rbegin = array.rbegin();

    EXPECT_FALSE(is_const(*rbegin));
    EXPECT_EQ(expected, &*rbegin);
}

TYPED_TEST(Array_Iterator, RBegin_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::rbegin_of(array);
    auto rbegin = array.rbegin();

    EXPECT_TRUE(is_const(*rbegin));
    EXPECT_EQ(expected, &*rbegin);
}

TYPED_TEST(Array_Iterator, CRBegin)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rbegin_of(array);
    auto crbegin = array.crbegin();

    EXPECT_TRUE(is_const(*crbegin));
    EXPECT_EQ(expected, &*crbegin);
}

TYPED_TEST(Array_Iterator, REnd_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rend()));
}

TYPED_TEST(Array_Iterator, REnd_Const_IsNoExcept)
{
    const auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.rend()));
}

TYPED_TEST(Array_Iterator, CREnd_IsNoExcept)
{
    auto array = TypeParam::make_array();
    EXPECT_TRUE(noexcept(array.crend()));
}

TYPED_TEST(Array_Iterator, REnd)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rend_of(array);
    auto rend = array.rend();

    EXPECT_FALSE(is_const(*rend));
    EXPECT_EQ(expected, &*rend);
}

TYPED_TEST(Array_Iterator, REnd_Const)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::rend_of(array);
    auto rend = array.rend();

    EXPECT_TRUE(is_const(*rend));
    EXPECT_EQ(expected, &*rend);
}

TYPED_TEST(Array_Iterator, CREnd)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::rend_of(array);
    auto crend = array.crend();

    EXPECT_TRUE(is_const(*crend));
    EXPECT_EQ(expected, &*crend);
}

TYPED_TEST(Array_Iterator, RBegin_REnd_Distance)
{
    auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.rbegin(), array.rend());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Iterator, RBegin_REnd_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.rbegin(), array.rend());

    EXPECT_EQ(expected, distance);
}

TYPED_TEST(Array_Iterator, CRBegin_CREnd_Const_Distance)
{
    const auto array = TypeParam::make_array();
    auto expected = TypeParam::size;
    auto distance = std::distance(array.crbegin(), array.crend());

    EXPECT_EQ(expected, distance);
}
