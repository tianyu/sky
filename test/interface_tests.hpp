#ifndef INTERFACE_TESTS_HPP
#define INTERFACE_TESTS_HPP

#include "gtest/gtest.h"
#include <tuple>
#include <type_traits>

template<size_t index, typename Output, typename... Msgs>
static
typename std::enable_if<(index >= sizeof...(Msgs))>::type
__outputMsgs(Output &, std::tuple<Msgs...> &)
{}

template<size_t index, typename Output, typename... Msgs>
static
typename std::enable_if<(index < sizeof...(Msgs))>::type
__outputMsgs(Output &out, std::tuple<Msgs...> &msgs)
{
    out << std::get<index>(msgs);
    __outputMsgs<index+1>(out, msgs);
}

template<typename Output, typename... Msgs>
static
Output &operator <<(Output &out, std::tuple<Msgs...> msgs)
{
    __outputMsgs<0>(out, msgs);
    return out;
}

template<typename T>
class InterfaceOf
{

    template<typename... Msgs>
    static void compare(bool expected, bool actual, Msgs... msgs)
    {
        static char const*const name = typeid(T).name();
        EXPECT_EQ(expected, actual)
                << name << " should "
                << (expected? "" : "not ")
                << "be " << std::make_tuple(msgs...) << '.';
    }

public:
    InterfaceOf() = delete;

    static void expect_default_constructible(bool expected = true)
    {
        compare(expected,
                std::is_default_constructible<T>::value,
                "default constructible");
    }

    template<typename... Args>
    static void expect_constructible(bool expected = true)
    {
        compare(expected,
                std::is_constructible<T, Args...>::value,
                "constructible with <",
                typeid(Args).name()...,
                ">");
    }

    static void expect_copy_constructible(bool expected = true)
    {
        compare(expected,
                std::is_copy_constructible<T>::value,
                "copy constructible");
    }

    static void expect_move_constructible(bool expected = true)
    {
        compare(expected,
                std::is_move_constructible<T>::value,
                "move constructible");
    }

    static void expect_copy_assignable(bool expected = true)
    {
        compare(expected,
                std::is_copy_assignable<T>::value,
                "copy assignable");
    }

    static void expect_move_assignable(bool expected = true)
    {
        compare(expected,
                std::is_move_assignable<T>::value,
                "move assignable");
    }
};

#endif // INTERFACE_TESTS_HPP
