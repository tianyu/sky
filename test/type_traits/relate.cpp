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

TEST(Relate, T_Forall_None)
{
    bool value = relate<same>::template op<int, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, T_Forall_U_True)
{
    bool value = relate<same>::template op<int, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, T_Forall_U_False)
{
    bool value = relate<same>::template op<int, forall<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, T_Forall_Us_True)
{
    bool value = relate<same>::
            template op<int, forall<int, int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, T_Forall_Us_False)
{
    bool value = relate<same>::
            template op<int, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, T_Exists_None)
{
    bool value = relate<same>::template op<int, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, T_Exists_U_True)
{
    bool value = relate<same>::template op<int, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, T_Exists_U_False)
{
    bool value = relate<same>::template op<int, exists<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, T_Exists_Us_True)
{
    bool value = relate<same>::
            template op<int, exists<double, int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, T_Exists_Us_False)
{
    bool value = relate<same>::
            template op<int, exists<double, double, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Forall_None_U)
{
    bool value = relate<same>::template op<forall<>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Forall_T_U_True)
{
    bool value = relate<same>::template op<forall<int>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Forall_T_U_False)
{
    bool value = relate<same>::template op<forall<double>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Forall_Ts_U_True)
{
    bool value = relate<same>::
            template op<forall<int, int, int>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Forall_Ts_U_False)
{
    bool value = relate<same>::
            template op<forall<int, double, int>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_U)
{
    bool value = relate<same>::template op<exists<>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_T_U_True)
{
    bool value = relate<same>::template op<exists<int>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_T_U_False)
{
    bool value = relate<same>::template op<exists<double>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_Ts_U_True)
{
    bool value = relate<same>::
            template op<exists<double, int, double>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_Ts_U_False)
{
    bool value = relate<same>::
            template op<exists<double, double, double>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_None_ForAll_None)
{
    bool value = relate<same>::
            template op<forall<>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_None_Exists_None)
{
    bool value = relate<same>::
            template op<forall<>, exists<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_None_ForAll_None)
{
    bool value = relate<same>::
            template op<exists<>, forall<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_Exists_None)
{
    bool value = relate<same>::
            template op<exists<>, exists<>>::value;
    EXPECT_FALSE(value);
}
