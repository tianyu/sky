#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

namespace sky {

template<typename Predicate, typename T, typename... Us>
struct predicate_and;

template<typename Predicate, typename T, typename First, typename... Rest>
struct predicate_and<Predicate, T, First, Rest...> :
        public std::integral_constant<bool,
            Predicate::template op<T, First>::value &&
            predicate_and<Predicate, T, Rest...>::value>
{};

template<typename Predicate, typename T>
struct predicate_and<Predicate, T> : public std::true_type
{};

template<typename Predicate, typename T, typename... Us>
struct predicate_or;

template<typename Predicate, typename T, typename First, typename... Rest>
struct predicate_or<Predicate, T, First, Rest...> :
        public std::integral_constant<bool,
            Predicate::template op<T, First>::value ||
            predicate_or<Predicate, T, Rest...>::value>
{};

template<typename Predicate, typename T>
struct predicate_or<Predicate, T> : public std::false_type
{};

namespace relation {

#define STD_BINARY_RELATION(name) \
struct name \
{ \
    template<typename T, typename U> \
    using op = std::name<T, U>; \
}

STD_BINARY_RELATION(is_same);
STD_BINARY_RELATION(is_base_of);
STD_BINARY_RELATION(is_convertible);

#undef STD_BINARY_RELATION

} // namespace relation

template<typename T, typename... Us>
using all_same = predicate_and<relation::is_same, T, Us...>;

template<typename T, typename... Us>
using exists_same = predicate_or<relation::is_same, T, Us...>;

template<typename T, typename... Us>
using all_base_of = predicate_and<relation::is_base_of, T, Us...>;

template<typename T, typename... Us>
using exists_base_of = predicate_or<relation::is_base_of, T, Us...>;

template<typename T, typename... Us>
using all_convertible = predicate_and<relation::is_convertible, T, Us...>;

template<typename T, typename... Us>
using exists_convertible = predicate_or<relation::is_convertible, T, Us...>;

} // namespace sky

#endif // TYPE_TRAITS_HPP
