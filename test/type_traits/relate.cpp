#include "gtest/gtest.h"

#include "sky/type_traits.hpp"

using namespace sky;

namespace {

struct truth
{
    template<typename...>
    using op = std::true_type;
};

struct untruth
{
    template<typename...>
    using op = std::false_type;
};

struct is_void
{
    template<typename T>
    using op = std::is_void<T>;
};

struct same
{
    template<typename T, typename U>
    using op = std::is_same<T,U>;
};

} // namespace

TEST(Relate0, True)
{
    bool value = relate<truth>::value;
    EXPECT_TRUE(value);
}

TEST(Relate0, False)
{
    bool value = relate<untruth>::value;
    EXPECT_FALSE(value);
}

TEST(Relate1, Default_True)
{
    bool value = relate<is_void, void>::value;
    EXPECT_TRUE(value);
}

TEST(Relate1, Default_False)
{
    bool value = relate<is_void, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate1, Forall_None)
{
    bool value = relate<is_void, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate1, Forall_T_True)
{
    bool value = relate<is_void, forall<void>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate1, Forall_Ts_True)
{
    bool value = relate<is_void, forall<void, void>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate1, Forall_T_False)
{
    bool value = relate<is_void, forall<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate1, Forall_Ts_False)
{
    bool value = relate<is_void, forall<void, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate1, Exists_None)
{
    bool value = relate<is_void, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate1, Exists_T_True)
{
    bool value = relate<is_void, exists<void>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate1, Exists_Ts_True)
{
    bool value = relate<is_void, exists<int, void>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate1, Exists_T_False)
{
    bool value = relate<is_void, exists<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate1, Exists_Ts_False)
{
    bool value = relate<is_void, exists<int, int>>::value;
    EXPECT_FALSE(value);
}


TEST(Relate2, Default_True)
{
    bool value = relate<same, int, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Default_False)
{
    bool value = relate<same, int, double>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, T_Forall_None)
{
    bool value = relate<same, int, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, T_Forall_U_True)
{
    bool value = relate<same, int, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, T_Forall_U_False)
{
    bool value = relate<same, int, forall<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, T_Forall_Us_True)
{
    bool value = relate<same,
            int, forall<int, int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, T_Forall_Us_False)
{
    bool value = relate<same,
            int, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, T_Exists_None)
{
    bool value = relate<same, int, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, T_Exists_U_True)
{
    bool value = relate<same, int, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, T_Exists_U_False)
{
    bool value = relate<same, int, exists<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, T_Exists_Us_True)
{
    bool value = relate<same,
            int, exists<double, int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, T_Exists_Us_False)
{
    bool value = relate<same,
            int, exists<double, double, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Forall_None_U)
{
    bool value = relate<same, forall<>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Forall_T_U_True)
{
    bool value = relate<same, forall<int>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Forall_T_U_False)
{
    bool value = relate<same, forall<double>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Forall_Ts_U_True)
{
    bool value = relate<same,
            forall<int, int, int>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Forall_Ts_U_False)
{
    bool value = relate<same,
            forall<int, double, int>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_U)
{
    bool value = relate<same, exists<>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_T_U_True)
{
    bool value = relate<same, exists<int>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_T_U_False)
{
    bool value = relate<same, exists<double>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_Ts_U_True)
{
    bool value = relate<same,
            exists<double, int, double>, int>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_Ts_U_False)
{
    bool value = relate<same,
            exists<double, double, double>, int>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_None_ForAll_None)
{
    bool value = relate<same,
            forall<>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_None_ForAll_U)
{
    bool value = relate<same,
            forall<>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_None_ForAll_Us)
{
    bool value = relate<same,
            forall<>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_ForAll_None)
{
    bool value = relate<same,
            forall<int>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_ForAll_U_True)
{
    bool value = relate<same,
            forall<int>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_ForAll_Us_True)
{
    bool value = relate<same,
            forall<int>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_ForAll_U_False)
{
    bool value = relate<same,
            forall<int>, forall<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_T_ForAll_Us_False)
{
    bool value = relate<same,
            forall<int>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_Ts_ForAll_None)
{
    bool value = relate<same,
            forall<int, double>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_Ts_ForAll_U_True)
{
    bool value = relate<same,
            forall<int, int>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_Ts_ForAll_Us_True)
{
    bool value = relate<same,
            forall<int, int>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_Ts_ForAll_U_False)
{
    bool value = relate<same,
            forall<int, int>, forall<double>>::value;
    EXPECT_FALSE(value);

    value = relate<same,
            forall<int, double>, forall<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_Ts_ForAll_Us_False)
{
    bool value = relate<same,
            forall<int, int>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);

    value = relate<same,
            forall<int, double>, forall<int, int, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_None_Exists_None)
{
    bool value = relate<same,
            forall<>, exists<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_None_Exists_U)
{
    bool value = relate<same,
            forall<>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_None_Exists_Us)
{
    bool value = relate<same,
            forall<>, exists<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_Exists_None)
{
    bool value = relate<same,
            forall<int>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_T_Exists_U_True)
{
    bool value = relate<same,
            forall<int>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_Exists_Us_True)
{
    bool value = relate<same,
            forall<int>, exists<double, int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_T_Exists_U_False)
{
    bool value = relate<same,
            forall<int>, exists<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_T_Exists_Us_False)
{
    bool value = relate<same,
            forall<int>, exists<double, long, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_Ts_Exists_None)
{
    bool value = relate<same,
            forall<int, int>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_Ts_Exists_U_True)
{
    bool value = relate<same,
            forall<int, int>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_Ts_Exists_Us_True)
{
    bool value = relate<same,
            forall<int, double>, exists<long, double, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, ForAll_Ts_Exists_U_False)
{
    bool value = relate<same,
            forall<int, int>, exists<double>>::value;
    EXPECT_FALSE(value);

    value = relate<same,
            forall<int, long>, exists<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, ForAll_Ts_Exists_Us_False)
{
    bool value = relate<same,
            forall<int, double>, exists<long, long, long>>::value;
    EXPECT_FALSE(value);

    value = relate<same,
            forall<int, double>, exists<long, double, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_ForAll_None)
{
    bool value = relate<same,
            exists<>, forall<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_ForAll_U)
{
    bool value = relate<same,
            exists<>, forall<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_ForAll_Us)
{
    bool value = relate<same,
            exists<>, forall<int, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_T_ForAll_None)
{
    bool value = relate<same,
            exists<int>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_T_ForAll_U_True)
{
    bool value = relate<same,
            exists<int>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_T_ForAll_Us_True)
{
    bool value = relate<same,
            exists<int>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_T_ForAll_U_False)
{
    bool value = relate<same,
            exists<int>, forall<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_T_ForAll_Us_False)
{
    bool value = relate<same,
            exists<int>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_Ts_ForAll_None)
{
    bool value = relate<same,
            exists<int, double>, forall<>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_Ts_ForAll_U_True)
{
    bool value = relate<same,
            exists<int, double>, forall<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_Ts_ForAll_Us_True)
{
    bool value = relate<same,
            exists<int, double>, forall<int, int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_Ts_ForAll_U_False)
{
    bool value = relate<same,
            exists<int, double>, forall<long>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_Ts_ForAll_Us_False)
{
    bool value = relate<same,
            exists<int, double>, forall<int, double, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_Exists_None)
{
    bool value = relate<same,
            exists<>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_Exists_U)
{
    bool value = relate<same,
            exists<>, exists<int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_None_Exists_Us)
{
    bool value = relate<same,
            exists<>, exists<int, int>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_T_Exists_None)
{
    bool value = relate<same,
            exists<int>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_T_Exists_U_True)
{
    bool value = relate<same,
            exists<int>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_T_Exists_Us_True)
{
    bool value = relate<same,
            exists<int>, exists<int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_T_Exists_U_False)
{
    bool value = relate<same,
            exists<int>, exists<double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_T_Exists_Us_False)
{
    bool value = relate<same,
            exists<int>, exists<long, double>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_Ts_Exists_None)
{
    bool value = relate<same,
            exists<int, double>, exists<>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_Ts_Exists_U_True)
{
    bool value = relate<same,
            exists<int, double>, exists<int>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_Ts_Exists_Us_True)
{
    bool value = relate<same,
            exists<int, double>, exists<int, double>>::value;
    EXPECT_TRUE(value);
}

TEST(Relate2, Exists_Ts_Exists_U_False)
{
    bool value = relate<same,
            exists<int, double>, exists<long>>::value;
    EXPECT_FALSE(value);
}

TEST(Relate2, Exists_Ts_Exists_Us_False)
{
    bool value = relate<same,
            exists<int, double>, exists<long, long>>::value;
    EXPECT_FALSE(value);
}
