#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

namespace sky {

template<typename T, typename... Us>
struct is_same;

template<typename T, typename First, typename... Rest>
struct is_same<T, First, Rest...> :
        public std::integral_constant<bool,
            std::is_same<T, First>::value &&
            sky::is_same<T, Rest...>::value>
{};

template<typename T, typename U>
struct is_same<T, U> : public std::is_same<T, U>
{};

} // namespace sky

#endif // TYPE_TRAITS_HPP
