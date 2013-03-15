#include "gtest/gtest.h"

#include "sky/tuple.hpp"

#include <functional>

using sky::invoke;

namespace { // Utility namespace

template<typename Return, typename... Args>
struct function_invoked
{
    static bool value;
};

template<typename Return, typename... Args>
bool function_invoked<Return, Args...>::value = false;

template <typename Return, typename... Args>
typename std::enable_if<
    !std::is_void<Return>::value,
    Return
>::type
function(Args...)
{
    function_invoked<Return, Args...>::value = true;
    return Return();
}

template <typename Return, typename... Args>
typename std::enable_if<
    std::is_void<Return>::value
>::type
function(Args...)
{
    function_invoked<void, Args...>::value = true;
}

template <typename Return, typename... Args>
struct functor
{
    mutable bool invoked = false;

    Return operator ()(Args...) const
    {
        invoked = true;
        return Return();
    }
};

template <typename... Args>
struct functor<void, Args...>
{
    mutable bool invoked = false;

    void operator ()(Args...) const
    {
        invoked = true;
    }
};

template <typename Return, typename... Args>
std::function<Return(Args...)>
stdfunc()
{
    return std::function<Return(Args...)>(function<Return, Args...>);
}

template<typename Return, typename... Args>
void expect_invoked(Return (*)(Args...))
{
    bool was_invoked = function_invoked<Return, Args...>::value;
    EXPECT_TRUE(was_invoked);
    function_invoked<Return, Args...>::value = false; // reset
}

template<typename Return, typename... Args>
void expect_invoked(functor<Return, Args...> const&func)
{
    bool was_invoked = func.invoked;
    EXPECT_TRUE(was_invoked);
    func.invoked = false; // reset
}

template<typename Return, typename... Args>
void expect_invoked(std::function<Return(Args...)> const&)
{
    bool was_invoked = function_invoked<Return, Args...>::value;
    EXPECT_TRUE(was_invoked);
    function_invoked<Return, Args...>::value = false; // reset
}

} // namespace

// No return value + No argument

TEST(Invoke, FunctionPointer_Void_Void)
{
    auto func = function<void>;
    invoke(func, std::tuple<>());
    expect_invoked(func);
}

TEST(Invoke, Functor_Void_Void)
{
    auto func = functor<void>();
    invoke(func, std::tuple<>());
    expect_invoked(func);
}

TEST(Invoke, Const_Functor_Void_Void)
{
    const auto func = functor<void>();
    invoke(func, std::tuple<>());
    expect_invoked(func);
}

TEST(Invoke, StdFunction_Void_Void)
{
    auto func = stdfunc<void>();
    invoke(func, std::tuple<>());
    expect_invoked(func);
}

// Return value + No argument

TEST(Invoke, FunctionPointer_Int_Void)
{
    auto func = function<int>;
    int ret = invoke(func, std::tuple<>()); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, Functor_Int_Void)
{
    auto func = functor<int>();
    int ret = invoke(func, std::tuple<>()); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, Const_Functor_Int_Void)
{
    const auto func = functor<int>();
    int ret = invoke(func, std::tuple<>()); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, StdFunction_Int_Void)
{
    auto func = stdfunc<int>();
    int ret = invoke(func, std::tuple<>()); (void)ret;
    expect_invoked(func);
}

// No return value + 1 argument

TEST(Invoke, FunctionPointer_Void_Double)
{
    auto func = function<void, double>;
    invoke(func, std::forward_as_tuple(2.0));
    expect_invoked(func);
}

TEST(Invoke, Functor_Void_Double)
{
    auto func = functor<void, double>();
    invoke(func, std::forward_as_tuple(2.0));
    expect_invoked(func);
}

TEST(Invoke, Const_Functor_Void_Double)
{
    const auto func = functor<void, double>();
    invoke(func, std::forward_as_tuple(2.0));
    expect_invoked(func);
}

TEST(Invoke, StdFunction_Void_Double)
{
    auto func = stdfunc<void, double>();
    invoke(func, std::forward_as_tuple(2.0));
    expect_invoked(func);
}

// Return value + 1 argument

TEST(Invoke, FunctionPointer_Int_Double)
{
    auto func = function<int, double>;
    int ret = invoke(func, std::forward_as_tuple(2.0)); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, Functor_Int_Double)
{
    auto func = functor<int, double>();
    int ret = invoke(func, std::forward_as_tuple(2.0)); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, Const_Functor_Int_Double)
{
    const auto func = functor<int, double>();
    int ret = invoke(func, std::forward_as_tuple(2.0)); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, StdFunction_Int_Double)
{
    auto func = stdfunc<int, double>();
    int ret = invoke(func, std::forward_as_tuple(2.0)); (void)ret;
    expect_invoked(func);
}

// Return value + multiple arguments

TEST(Invoke, FunctionPointer_Int_Double_Int)
{
    int i = 0;
    auto func = function<int, double, int*, int&>;
    int ret = invoke(func, std::forward_as_tuple(2.0, &i, i)); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, Functor_Int_Double_Int)
{
    int i = 0;
    auto func = functor<int, double, int*, int&>();
    int ret = invoke(func, std::forward_as_tuple(2.0, &i, i)); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, Const_Functor_Int_Double_Int)
{
    int i = 0;
    const auto func = functor<int, double, int*, int&>();
    int ret = invoke(func, std::forward_as_tuple(2.0, &i, i)); (void)ret;
    expect_invoked(func);
}

TEST(Invoke, StdFunction_Int_Double_Int)
{
    int i = 0;
    auto func = stdfunc<int, double, int*, int&>();
    int ret = invoke(func, std::forward_as_tuple(2.0, &i, i)); (void)ret;
    expect_invoked(func);
}

// The following are tests for the efficiency of sky::invoke

namespace {

struct TestObject
{
    TestObject() :
        copies(0),
        moves(0)
    {}

    TestObject(TestObject const&o) :
        copies(o.copies + 1),
        moves(o.moves)
    {}

    TestObject(TestObject &&o) :
        copies(o.copies),
        moves(o.moves + 1)
    {}

    const int copies, moves;
};

template <typename T>
std::tuple<int, int>
test_capture(T o)
{
    return std::make_tuple(o.copies, o.moves);
}

TestObject test_return()
{
    return TestObject();
}

} // namespace

TEST(Invoke, CaptureBy_Ref)
{
    TestObject o;
    int copies, moves;
    std::tie(copies, moves) =
            invoke(&test_capture<TestObject&>,
                   std::forward_as_tuple(o));
    EXPECT_EQ(0, copies);
    EXPECT_EQ(0, moves);
}

TEST(Invoke, CaptureBy_RefRef)
{
    int copies, moves;
    std::tie(copies, moves) =
            invoke(&test_capture<TestObject&&>,
                   std::forward_as_tuple(TestObject()));
    EXPECT_EQ(0, copies);
    EXPECT_EQ(0, moves);
}

TEST(Invoke, CaptureBy_ConstRef)
{
    int copies, moves;
    std::tie(copies, moves) =
            invoke(&test_capture<TestObject const&>,
                   std::forward_as_tuple(TestObject()));
    EXPECT_EQ(0, copies);
    EXPECT_EQ(0, moves);
}

TEST(Invoke, CaptureBy_Value)
{
    int copies, moves;
    std::tie(copies, moves) =
            invoke(&test_capture<TestObject>,
                   std::forward_as_tuple(TestObject()));
    EXPECT_EQ(0, copies);
    EXPECT_EQ(1, moves); // Forwarding disables RVO, so we expect a move.
}

TEST(Invoke, Return_Value)
{
    TestObject test =
            invoke(&test_return,
                   std::tuple<>());
    EXPECT_EQ(0, test.copies); // Copies should be elided.
    EXPECT_EQ(0, test.moves);
}
