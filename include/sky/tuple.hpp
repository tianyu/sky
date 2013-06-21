#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <cstddef>
#include <tuple>
#include <utility>

#include "type_traits.hpp"

namespace sky {

namespace _ {

template<typename T, typename... Us>
struct unique_index_of;

template<typename T>
struct unique_index_of<T>
{
    static_assert(!std::is_same<T, T>::value, // Condition always fails.
                    "Type does not exist.");
};

template<typename T, typename... Us>
struct unique_index_of<T, T, Us...> :
    public std::integral_constant<std::size_t, 0>
{
    static_assert(!sky::is_same<T, sky::exists<Us...>>::value,
                    "Type is not unique.");
};

template<typename T, typename U, typename... Us>
struct unique_index_of<T, U, Us...> :
    public std::integral_constant<std::size_t,
        1 + unique_index_of<T, Us...>::value>
{};

} // namespace _

/**
 * Gets the object of type T from within the tuple.
 *
 * If there is no object of type T, or if there are multiple objects of type T,
 * then this function fails to compile.
 */
template<typename T, typename... Us>
T &get(std::tuple<Us...> &t)
{
    return std::get<_::unique_index_of<T, Us...>::value>(t);
}

/**
 * Gets the object of type T from within the tuple.
 *
 * If there is no object of type T, or if there are multiple objects of type T,
 * then this function fails to compile.
 */
template<typename T, typename... Us>
T &&get(std::tuple<Us...> &&t)
{
    return std::get<_::unique_index_of<T, Us...>::value>(std::move(t));
}

/**
 * Gets the object of type T from within the tuple.
 *
 * If there is no object of type T, or if there are multiple objects of type T,
 * then this function fails to compile.
 */
template<typename T, typename... Us>
T const&get(std::tuple<Us...> const&t)
{
    return std::get<_::unique_index_of<T, Us...>::value>(t);
}

/**
 * @defgroup param_tuples Parameter Tuples
 *
 * Parameter forwarding is useful when one function is required to call another
 * overloaded function. These functions can be regular functions, callable
 * objects or even constructors. Variadic templates are sufficient if only one
 * overloaded function is being called. However if multiple overloaded functions
 * are to be called, then it's impossible to use only one variadic template.
 *
 * One solution is to use store the different sets of parameters in tuples, for
 * example:
 *
 *     template <typename... Args1, typename... Args2>
 *     Object foo(std::tuple<Args1...> args1, std::tuple<Args2...> args2) {
 *         // Call bar() with arguments in args1.
 *         Object o = // Consturct an Object wth arguments in args2.
 *         return o;
 *     }
 *
 * In this case, `foo` takes two arguments, `args1` and `args2`, both of which
 * are tuples of any length. It is also clear which tuple of arguments are to be
 * used with function. The only problem is that there is no convenient way to
 * re-expand the arguments at the function's call site once they've been placed
 * in a tuple.
 *
 * The invoke() and make() functions provided here make the above code possible
 * and convenient:
 *
 *     template <typename... Args1, typename... Args2>
 *     Object foo(std::tuple<Args1...> args1, std::tuple<Args2...> args2) {
 *         invoke(&bar, args1);
 *         Object o = make<Object>(args2);
 *         return o;
 *     }
 */

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

/**
 * @brief Invokes a function with the parameters given in the tuple.
 *
 * @ingroup param_tuples
 *
 * @param f The function or functor to invoke.
 * @param args The arguments of the function, as a tuple.
 * @return The value of returned by f(args...), if any.
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

/**
 * @brief make Constructs an object with parameters given in a tuple.
 *
 * @ingroup param_tuples
 *
 * @param args The arguments for the object's constructor, as a tuple.
 * @return The constructed object.
 */
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
