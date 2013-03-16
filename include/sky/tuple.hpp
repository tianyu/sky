#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace sky {

namespace _ {

// Declarations of helper functions/structs //

template <int... Is> struct int_list {};

template <size_t MAX> struct index_up_to;
template <> struct index_up_to<0>;

template <typename F, typename... Args, int... Index>
void
invoke_helper_no_return(F&&, std::tuple<Args...>&, int_list<Index...>);

template <typename F, typename... Args, int... Index>
typename std::result_of<F(Args...)>::type
invoke_helper_with_return(F&&, std::tuple<Args...>&, int_list<Index...>);

template <typename T, typename... Args, int... Index>
T make_helper(std::tuple<Args...>&, int_list<Index...>);

} // namespace _

/** @{
 * @brief Invokes a function with the parameters given in the tuple.
 *
 * Suppose we had a function foo,() which calls an overloaded function bar().
 * It is possible to write only one definition of foo() that is able to call any
 * overload of bar() using variadic templates with perfect forwarding:
 *
 * template <typename... Args>
 * void foo(Args &&... args) {
 *    bar(std::forward<Args>(args)...);
 * }
 *
 * However, if foo() is supposed to call two overloaded functions,
 * bar1() and bar2(), then it becomes impossible to write foo() using only one
 * variadic template. A solution would be to use two variadic templates with
 * the parameters passed in tuples:
 *
 * template <typename... Args1, typename Args2>
 * void foo(std::tuple<Args1...> &&args1, std::tuple<Args2...> &&args2)
 *
 * However, there is no standard way of calling bar1() or bar2() with the
 * arguments stored in tuples. This function provides such a solution:
 *
 * invoke(&bar1, args1); // Like bar1(args1...)
 * invoke(&bar2, args2); // Like bar2(args2...)
 *
 * Furthermore, the return value of invoking bar1() and bar2() can also be used.
 *
 * Note: Since foo() takes two tuples instead of one variadic list of arguments,
 * it should be called in the following way, if perfect forwarding is desired:
 *
 *  foo(std::forward_as_tuple(args, for, bar1),
 *      std::forward_as_tuple(other, args, for, bar, 2));
 *
 * @param f The function or functor to invoke.
 * @param args The arguments of the function, as a tuple.
 * @return The value the function returns, if any.
 */
template <typename F, typename... Args>
typename std::enable_if<
    std::is_void< typename std::result_of<F(Args...)>::type >::value
>::type
invoke(F &&f, std::tuple<Args...> args)
{
    using namespace _;
    typedef typename index_up_to<sizeof...(Args)>::type Index;
    invoke_helper_no_return(std::forward<F>(f), args, Index());
}

template <typename F, typename... Args>
typename std::enable_if<
    !std::is_void< typename std::result_of<F(Args...)>::type >::value,
    typename std::result_of<F(Args...)>::type
>::type
invoke(F &&f, std::tuple<Args...> args)
{
    using namespace _;
    typedef typename index_up_to<sizeof...(Args)>::type Index;
    return invoke_helper_with_return(std::forward<F>(f), args, Index());
}
/// @}

template <typename T, typename... Args>
T make(std::tuple<Args...> args)
{
    using namespace _;
    typedef typename index_up_to<sizeof...(Args)>::type Index;
    return make_helper<T>(args, Index());
}

namespace _ {

// Implementation of helper declarations //

template<>
struct index_up_to<0>
{
    typedef int_list<> type;
};

template <size_t MAX>
struct index_up_to
{
private:
    template <size_t I, size_t... List>
    struct helper
    {
        typedef typename helper<I-1, I, List...>::type type;
    };

    template <size_t... List>
    struct helper<0, List...>
    {
        typedef int_list<0, List...> type;
    };

public:
    typedef typename helper<MAX-1>::type type;
};

template <typename F, typename... Args, int... Index>
void
invoke_helper_no_return(F &&f, std::tuple<Args...> &args, int_list<Index...>)
{
    /*
     * Generate the arguments:
     * get<0>(args), get<1>(args), ...
     * And forward them to the function f.
     */
    f(std::forward<Args>(std::get<Index>(args))...);
}

template <typename F, typename... Args, int... Index>
typename std::result_of<F(Args...)>::type
invoke_helper_with_return(F &&f, std::tuple<Args...> &args, int_list<Index...>)
{
    /*
     * Generate the arguments:
     * get<0>(args), get<1>(args), ...
     * And forward them to the function f.
     */
    return f(std::forward<Args>(std::get<Index>(args))...);
}

template <typename T, typename... Args, int... Index>
T make_helper(std::tuple<Args...> &args, int_list<Index...>)
{
    /*
     * Generate the arguments:
     * get<0>(args), get<1>(args), ...
     * And forward them to the constructor.
     */
    return T(std::forward<Args>(std::get<Index>(args))...);
}

} // namespace _

} // namespace sky

#endif // TUPLE_HPP
