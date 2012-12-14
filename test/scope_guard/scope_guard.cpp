#include "gtest/gtest.h"
#include "sky/scope_guard.hpp"

#include <functional>

using sky::scope_guard;

// Definitions of some callable things for testing
namespace {

int returnint() { return 5; }

void returnvoid() { return; }

int &increment(int &n) { return ++n; }

class Functor {
public:
    void operator ()() {}
};

} // namespace

TEST(ScopeGuard, ConstructWithFunctionPtrInt)
{
    scope_guard(returnint);
}

TEST(ScopeGuard, ConstructWithFunctionPtrInt_Lvalue)
{
    auto todo = returnint;
    scope_guard(todo);
}

TEST(ScopeGuard, ConstructWithFunctionPtrVoid)
{
    scope_guard(returnvoid);
}

TEST(ScopeGuard, ConstructWithFunctionPtrVoid_Lvalue)
{
    auto todo = returnvoid;
    scope_guard(todo);
}

TEST(ScopeGuard, ConstructWithFunctor)
{
    scope_guard(Functor());
}

TEST(ScopeGuard, ConstructWithFunctor_Lvalue)
{
    auto todo = Functor();
    scope_guard(todo);
}

TEST(ScopeGuard, ConstructWithFunction)
{
    scope_guard(std::function<int()>(returnint));
}

TEST(ScopeGuard, ConstructWithFunction_Lvalue)
{
    auto todo = std::function<int()>(returnint);
    scope_guard(todo);
}

TEST(ScopeGuard, ConstructWithBind)
{
    int n = 0;
    scope_guard(std::bind(increment, std::ref(n)));
}

TEST(ScopeGuard, ConstructWithBind_Lvalue)
{
    int n = 0;
    auto todo = std::bind(increment, std::ref(n));
    scope_guard(todo);
}

TEST(ScopeGuard, PerformOnce)
{
    int n = 0;
    {
        auto g = scope_guard(std::bind(increment, std::ref(n)));
        ASSERT_EQ(0, n);
        (void)g;
    }
    EXPECT_EQ(1, n);
}

TEST(ScopeGuard, PerformOnce_Lvalue)
{
    int n = 0;
    auto todo = std::bind(increment, std::ref(n));
    {
        auto g = scope_guard(todo);
        ASSERT_EQ(0, n);
        (void)g;
    }
    EXPECT_EQ(1, n);
}

TEST(ScopeGuard, PerformTwice)
{
    int n = 0;
    {
        auto g1 = scope_guard(std::bind(increment, std::ref(n)));
        auto g2 = scope_guard(std::bind(increment, std::ref(n)));
        ASSERT_EQ(0, n);
        (void)g1;
        (void)g2;
    }
    EXPECT_EQ(2, n);
}

TEST(ScopeGuard, PerformTwice_Lvalue)
{
    int n = 0;
    auto todo = std::bind(increment, std::ref(n));
    {
        auto g1 = scope_guard(todo);
        auto g2 = scope_guard(todo);
        ASSERT_EQ(0, n);
        (void)g1;
        (void)g2;
    }
    EXPECT_EQ(2, n);
}

TEST(ScopeGuard, DismissOnce)
{
    int n = 0;
    {
        auto g1 = scope_guard(std::bind(increment, std::ref(n)));
        auto g2 = scope_guard(std::bind(increment, std::ref(n)));
        ASSERT_EQ(0, n);
        (void)g1;
        g2.dismiss();
    }
    EXPECT_EQ(1, n);
}

TEST(ScopeGuard, DismissOnce_Lvalue)
{
    int n = 0;
    auto todo = std::bind(increment, std::ref(n));
    {
        auto g1 = scope_guard(todo);
        auto g2 = scope_guard(todo);
        ASSERT_EQ(0, n);
        (void)g1;
        g2.dismiss();
    }
    EXPECT_EQ(1, n);
}

TEST(ScopeGuard, DismissTwice)
{
    int n = 0;
    {
        auto g1 = scope_guard(std::bind(increment, std::ref(n)));
        auto g2 = scope_guard(std::bind(increment, std::ref(n)));
        ASSERT_EQ(0, n);
        g1.dismiss();
        g2.dismiss();
    }
    EXPECT_EQ(0, n);
}

TEST(ScopeGuard, DismissTwice_Lvalue)
{
    int n = 0;
    auto todo = std::bind(increment, std::ref(n));
    {
        auto g1 = scope_guard(todo);
        auto g2 = scope_guard(todo);
        ASSERT_EQ(0, n);
        g1.dismiss();
        g2.dismiss();
    }
    EXPECT_EQ(0, n);
}
