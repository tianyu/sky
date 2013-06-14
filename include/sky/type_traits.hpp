#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

/**
 * @defgroup type_trait Type Traits
 */

namespace sky {
namespace predicate {

#define STD_UNARY_PREDICATE(name) \
struct name \
{ \
    template<typename T> \
    using op = std::name<T>; \
}

#undef STD_UNARY_PREDICATE

#define STD_BINARY_PREDICATE(name) \
struct name \
{ \
    template<typename T, typename U> \
    using op = std::name<T,U>; \
}

STD_BINARY_PREDICATE(is_same);

STD_BINARY_PREDICATE(is_base_of);

struct is_derived_from
{
    template<typename T, typename U>
    using op = std::is_base_of<U,T>;
};

STD_BINARY_PREDICATE(is_convertible);

struct is_convertible_from
{
    template<typename T, typename U>
    using op = std::is_convertible<U,T>;
};

#undef STD_BINARY_PREDICATE

} // namespace predicate

template<typename... Ts>
struct exists {};

template<typename... Ts>
struct forall {};

template<typename Predicate, typename T>
struct curry
{
    template<typename... Ts>
    struct op : public Predicate::template op<T, Ts...>
    {};
};

template<typename Predicate, typename... Args>
struct relate;

template<typename Predicate>
struct relate<Predicate> : public Predicate::template op<>
{};

template<typename Predicate, typename Arg, typename... Args>
struct relate<Predicate, Arg, Args...> : public
    relate<curry<Predicate, Arg>, Args...>
{};

template<typename Predicate, typename... Args>
struct relate<Predicate, forall<>, Args...> : public std::true_type
{};

template<typename Predicate, typename T, typename... Ts, typename... Args>
struct relate<Predicate, forall<T, Ts...>, Args...> : public
    std::integral_constant<bool,
        relate<Predicate, T, Args...>::value &&
        relate<Predicate, forall<Ts...>, Args...>::value>
{};

template<typename Predicate, typename... Args>
struct relate<Predicate, exists<>, Args...> : std::false_type
{};

template<typename Predicate, typename T, typename... Ts, typename... Args>
struct relate<Predicate, exists<T, Ts...>, Args...> : public
    std::integral_constant<bool,
        relate<Predicate, T, Args...>::value ||
        relate<Predicate, exists<Ts...>, Args...>::value>
{};

#define BINARY_RELATION(name) \
template<typename T, typename U> \
using name = relate<predicate::name, T,U>

BINARY_RELATION(is_same);
BINARY_RELATION(is_base_of);
BINARY_RELATION(is_derived_from);
BINARY_RELATION(is_convertible);
BINARY_RELATION(is_convertible_from);

#undef BINARY_RELATION

} // namespace sky

#endif // TYPE_TRAITS_HPP
