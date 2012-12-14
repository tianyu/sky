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

TEST(ScopeGuard, ConstructWithFunctionPtrVoid)
{
    scope_guard(returnvoid);
}

TEST(ScopeGuard, ConstructWithFunctor)
{
    scope_guard(Functor());
}

TEST(ScopeGuard, ConstructWithFunction)
{
    scope_guard(std::function<int()>(returnint));
}

TEST(ScopeGuard, ConstructWithBind)
{
    int n = 0;
    scope_guard(std::bind(increment, std::ref(n)));
}

TEST(ScopeGuard, PerformOnce)
{
    int n = 0;
    {
        auto g = scope_guard(std::bind(increment, std::ref(n)));
    }
    EXPECT_EQ(1, n);
}

TEST(ScopeGuard, PerformTwice)
{
    int n = 0;
    {
        auto g1 = scope_guard(std::bind(increment, std::ref(n)));
        auto g2 = scope_guard(std::bind(increment, std::ref(n)));
    }
    EXPECT_EQ(2, n);
}

TEST(ScopeGuard, DismissOnce)
{
    int n = 0;
    {
        auto g1 = scope_guard(std::bind(increment, std::ref(n)));
        auto g2 = scope_guard(std::bind(increment, std::ref(n)));
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
        g1.dismiss();
        g2.dismiss();
    }
    EXPECT_EQ(0, n);
}
