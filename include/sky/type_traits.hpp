#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

/**
 * @defgroup type_trait Type Traits
 */

namespace sky {
namespace predicate {

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
struct _relate;

template<typename Predicate>
struct _relate<Predicate> : public Predicate::template op<>
{};

template<typename Predicate, typename Arg, typename... Args>
struct _relate<Predicate, Arg, Args...> : public _relate<curry<Predicate, Arg>, Args...>
{};

template<typename Predicate, typename... Args>
struct _relate<Predicate, forall<>, Args...> : public std::true_type
{};

template<typename Predicate, typename T, typename... Ts, typename... Args>
struct _relate<Predicate, forall<T, Ts...>, Args...> : public
        std::integral_constant<bool,
        _relate<Predicate, T, Args...>::value &&
        _relate<Predicate, forall<Ts...>, Args...>::value>
{};

template<typename Predicate, typename... Args>
struct _relate<Predicate, exists<>, Args...> : std::false_type
{};

template<typename Predicate, typename T, typename... Ts, typename... Args>
struct _relate<Predicate, exists<T, Ts...>, Args...> : public
        std::integral_constant<bool,
        _relate<Predicate, T, Args...>::value ||
        _relate<Predicate, exists<Ts...>, Args...>::value>
{};

template<typename Predicate>
struct relate
{
private:

    template<typename T, typename U>
    struct op_base : public Predicate::template op<T,U>
    {};

    template<typename T>
    struct op_base<T, forall<>> : public std::true_type
    {};

    template<typename T, typename U, typename... Us>
    struct op_base<T, forall<U, Us...>> : public
        std::integral_constant<bool,
            Predicate::template op<T,U>::value &&
            op_base<T, forall<Us...>>::value>
    {};

    template<typename T>
    struct op_base<T, exists<>> : public std::false_type
    {};

    template<typename T, typename U, typename... Us>
    struct op_base<T, exists<U, Us...>> : public
        std::integral_constant<bool,
            Predicate::template op<T,U>::value ||
            op_base<T, exists<Us...>>::value>
    {};

public:

    template<typename T, typename U>
    struct op : public op_base<T, U>
    {};

    template<typename U>
    struct op<forall<>, U> : public std::true_type
    {};

    template<typename T, typename... Ts, typename U>
    struct op<forall<T, Ts...>, U> : public
        std::integral_constant<bool,
            op_base<T,U>::value &&
            op<forall<Ts...>, U>::value>
    {};

    template<typename U>
    struct op<exists<>, U> : public std::false_type
    {};

    template<typename T, typename... Ts, typename U>
    struct op<exists<T, Ts...>, U> : public
        std::integral_constant<bool,
            op_base<T,U>::value ||
            op<exists<Ts...>, U>::value>
    {};

};

#define BINARY_RELATION(name) \
template<typename T, typename U> \
using name = relate<predicate::name>::template op<T,U>

BINARY_RELATION(is_same);
BINARY_RELATION(is_base_of);
BINARY_RELATION(is_derived_from);
BINARY_RELATION(is_convertible);
BINARY_RELATION(is_convertible_from);

#undef BINARY_RELATION

} // namespace sky

#endif // TYPE_TRAITS_HPP
