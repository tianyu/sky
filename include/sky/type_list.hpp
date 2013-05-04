#ifndef TYPE_LIST_HPP
#define TYPE_LIST_HPP

#include <cstddef>

namespace sky {

template<typename T, T... Ts>
struct type_list
{};

/**
 * @brief A template list of indexes.
 */
template<size_t... Is>
using index_list = type_list<size_t, Is...>;

template<size_t Begin, size_t End>
struct index_range
{
    static_assert(Begin <= End,
        "index_range: Begin cannot exceed End.");

private:

    template<size_t First, size_t... Rest>
    struct helper
    {
        using type = typename helper<First-1, First, Rest...>::type;
    };

    template<size_t... Rest>
    struct helper<Begin, Rest...>
    {
        using type = index_list<Begin, Rest...>;
    };

public:
    using type = typename helper<End-1>::type;
};

template<size_t N>
struct index_range<N,N>
{
    using type = index_list<>;
};

} // namespace sky

#endif // TYPE_LIST_HPP
