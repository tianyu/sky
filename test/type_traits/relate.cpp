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

TEST(Relate, ForAll_None_ForAll_U)
{
    bool value = relate<same>::
            template op<forall<>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_None_ForAll_Us)
{
    bool value = relate<same>::
            template op<forall<>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_ForAll_None)
{
    bool value = relate<same>::
            template op<forall<int>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_ForAll_U_True)
{
    bool value = relate<same>::
            template op<forall<int>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_ForAll_Us_True)
{
    bool value = relate<same>::
            template op<forall<int>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_ForAll_U_False)
{
    bool value = relate<same>::
            template op<forall<int>, forall<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_T_ForAll_Us_False)
{
    bool value = relate<same>::
            template op<forall<int>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_Ts_ForAll_None)
{
    bool value = relate<same>::
            template op<forall<int, double>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_Ts_ForAll_U_True)
{
    bool value = relate<same>::
            template op<forall<int, int>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_Ts_ForAll_Us_True)
{
    bool value = relate<same>::
            template op<forall<int, int>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_Ts_ForAll_U_False)
{
    bool value = relate<same>::
            template op<forall<int, int>, forall<double>>::value;
    EXPECT_FALSE(value);

    value = relate<same>::
            template op<forall<int, double>, forall<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_Ts_ForAll_Us_False)
{
    bool value = relate<same>::
            template op<forall<int, int>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);

    value = relate<same>::
            template op<forall<int, double>, forall<int, int, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_None_Exists_None)
{
    bool value = relate<same>::
            template op<forall<>, exists<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_None_Exists_U)
{
    bool value = relate<same>::
            template op<forall<>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_None_Exists_Us)
{
    bool value = relate<same>::
            template op<forall<>, exists<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_Exists_None)
{
    bool value = relate<same>::
            template op<forall<int>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_T_Exists_U_True)
{
    bool value = relate<same>::
            template op<forall<int>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_Exists_Us_True)
{
    bool value = relate<same>::
            template op<forall<int>, exists<double, int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_T_Exists_U_False)
{
    bool value = relate<same>::
            template op<forall<int>, exists<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_T_Exists_Us_False)
{
    bool value = relate<same>::
            template op<forall<int>, exists<double, long, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_Ts_Exists_None)
{
    bool value = relate<same>::
            template op<forall<int, int>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_Ts_Exists_U_True)
{
    bool value = relate<same>::
            template op<forall<int, int>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_Ts_Exists_Us_True)
{
    bool value = relate<same>::
            template op<forall<int, double>, exists<long, double, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, ForAll_Ts_Exists_U_False)
{
    bool value = relate<same>::
            template op<forall<int, int>, exists<double>>::value;
    EXPECT_FALSE(value);

    value = relate<same>::
            template op<forall<int, long>, exists<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, ForAll_Ts_Exists_Us_False)
{
    bool value = relate<same>::
            template op<forall<int, double>, exists<long, long, long>>::value;
    EXPECT_FALSE(value);

    value = relate<same>::
            template op<forall<int, double>, exists<long, double, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_ForAll_None)
{
    bool value = relate<same>::
            template op<exists<>, forall<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_ForAll_U)
{
    bool value = relate<same>::
            template op<exists<>, forall<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_ForAll_Us)
{
    bool value = relate<same>::
            template op<exists<>, forall<int, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_T_ForAll_None)
{
    bool value = relate<same>::
            template op<exists<int>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_T_ForAll_U_True)
{
    bool value = relate<same>::
            template op<exists<int>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_T_ForAll_Us_True)
{
    bool value = relate<same>::
            template op<exists<int>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_T_ForAll_U_False)
{
    bool value = relate<same>::
            template op<exists<int>, forall<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_T_ForAll_Us_False)
{
    bool value = relate<same>::
            template op<exists<int>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_Ts_ForAll_None)
{
    bool value = relate<same>::
            template op<exists<int, double>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_Ts_ForAll_U_True)
{
    bool value = relate<same>::
            template op<exists<int, double>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_Ts_ForAll_Us_True)
{
    bool value = relate<same>::
            template op<exists<int, double>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_Ts_ForAll_U_False)
{
    bool value = relate<same>::
            template op<exists<int, double>, forall<long>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_Ts_ForAll_Us_False)
{
    bool value = relate<same>::
            template op<exists<int, double>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_Exists_None)
{
    bool value = relate<same>::
            template op<exists<>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_Exists_U)
{
    bool value = relate<same>::
            template op<exists<>, exists<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_None_Exists_Us)
{
    bool value = relate<same>::
            template op<exists<>, exists<int, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_T_Exists_None)
{
    bool value = relate<same>::
            template op<exists<int>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_T_Exists_U_True)
{
    bool value = relate<same>::
            template op<exists<int>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_T_Exists_Us_True)
{
    bool value = relate<same>::
            template op<exists<int>, exists<int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_T_Exists_U_False)
{
    bool value = relate<same>::
            template op<exists<int>, exists<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_T_Exists_Us_False)
{
    bool value = relate<same>::
            template op<exists<int>, exists<long, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_Ts_Exists_None)
{
    bool value = relate<same>::
            template op<exists<int, double>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_Ts_Exists_U_True)
{
    bool value = relate<same>::
            template op<exists<int, double>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_Ts_Exists_Us_True)
{
    bool value = relate<same>::
            template op<exists<int, double>, exists<int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate, Exists_Ts_Exists_U_False)
{
    bool value = relate<same>::
            template op<exists<int, double>, exists<long>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate, Exists_Ts_Exists_Us_False)
{
    bool value = relate<same>::
            template op<exists<int, double>, exists<long, long>>::value;
    EXPECT_FALSE(value);
}
