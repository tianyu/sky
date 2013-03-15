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

} // namespace _

template <typename F, typename... Args>
typename std::enable_if<
    std::is_void< typename std::result_of<F(Args...)>::type >::value
>::type
invoke(F &&f, std::tuple<Args...> &&args)
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
invoke(F &&f, std::tuple<Args...> &&args)
{
    using namespace _;
    typedef typename index_up_to<sizeof...(Args)>::type Index;
    return invoke_helper_with_return(std::forward<F>(f), args, Index());
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
    f(std::forward<
        typename std::tuple_element<Index, std::tuple<Args...>>::type
      >(std::get<Index>(args))...);
}

template <typename F, typename... Args, int... Index>
typename std::result_of<F(Args...)>::type
invoke_helper_with_return(F &&f, std::tuple<Args...> &args, int_list<Index...>)
{
    return f(std::forward<
                typename std::tuple_element<Index, std::tuple<Args...>>::type
             >(std::get<Index>(args))...);
}

} // namespace _

} // namespace sky

#endif // TUPLE_HPP
