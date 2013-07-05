#ifndef TYPE_LIST_HPP
#define TYPE_LIST_HPP

#include <cstddef>
#include <type_traits>

namespace sky {

/**
 * A templated list of values of type T.
 */
template<typename T, T... Ts>
struct type_list
{};

/**
 * @typedef type_list<std::size_t, Is...> index_list;
 * A template list of indexes.
 */
template<std::size_t... Is>
using index_list = type_list<std::size_t, Is...>;

/**
 * Contains an index_list with parameters `Begin, Begin+1, ..., End-1`.
 *
 * #### Example
 *
 *     typename index_range<0, 5>::type; // Same as index_list<0, 1, 2, 3, 4>
 */
template<std::size_t Begin, std::size_t End>
struct index_range
{
    static_assert(Begin <= End,
        "index_range: Begin cannot exceed End.");

private:

    template<std::size_t First, std::size_t... Rest>
    struct helper
    {
        using type = typename helper<First-1, First, Rest...>::type;
    };

    template<std::size_t... Rest>
    struct helper<Begin, Rest...>
    {
        using type = index_list<Begin, Rest...>;
    };

public:
    using type = typename helper<End-1>::type;
};

template<std::size_t N>
struct index_range<N,N>
{
    using type = index_list<>;
};

template<typename T, T... Ts>
struct product;

template<typename T>
struct product<type_list<T>> : public std::integral_constant<T, T(1)>
{};

template<typename T, T T1, T... Ts>
struct product<type_list<T, T1, Ts...>> :
    public std::integral_constant<T, T1 * product<type_list<T, Ts...>>::value>
{};

} // namespace sky

#endif // TYPE_LIST_HPP
