#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

/**
 * @defgroup type_trait Type Traits
 */

namespace sky {

/**
 * @brief Boolean type trait that is true if and only if
 * for all `U` in `U`s such that `Predicate(T,U)` is true.
 *
 * `Predicate` should be a boolean binary type trait.
 * In other words, the expression `Predicate::template op<T,U>::value` should
 * be compile-time convertible to a boolean value.
 *
 * Literally, the computed value of `predicate_and<P, T, U1, U2, ..., Un>` is:
 *
 *     P::template op<T, U1>::value &&
 *     P::template op<T, U2>::value &&
 *     ...
 *     P::template op<T, Un>::value;
 *
 * The default value of predicate_and, when there are no `U`s, is `true`.
 *
 * @ingroup type_trait
 */
template<typename Predicate, typename T, typename... Us>
struct predicate_and;

template<typename Predicate, typename T, typename First, typename... Rest>
struct predicate_and<Predicate, T, First, Rest...> : public
    std::integral_constant<bool,
        Predicate::template op<T, First>::value &&
        predicate_and<Predicate, T, Rest...>::value>
{};

template<typename Predicate, typename T>
struct predicate_and<Predicate, T> : public
    std::true_type
{};

/**
 * @brief Boolean type trait that is true if and only if
 * there exists `U` in `U`s such that `Predicate(T,U)` is true.
 *
 * `Predicate` should be a boolean binary type trait.
 * In other words, the expression `Predicate::template op<T,U>::value` should
 * be compile-time convertible to a boolean value.
 *
 * Literally, the computed value of `predicate_or<P, T, U1, U2, ..., Un>` is:
 *
 *     P::template op<T, U1>::value ||
 *     P::template op<T, U2>::value ||
 *     ...
 *     P::template op<T, Un>::value;
 *
 * The default value of predicate_or, when there are no `U`s, is `false`.
 *
 * @ingroup type_trait
 */
template<typename Predicate, typename T, typename... Us>
struct predicate_or;

template<typename Predicate, typename T, typename First, typename... Rest>
struct predicate_or<Predicate, T, First, Rest...> : public
    std::integral_constant<bool,
        Predicate::template op<T, First>::value ||
        predicate_or<Predicate, T, Rest...>::value>
{};

template<typename Predicate, typename T>
struct predicate_or<Predicate, T> : public
    std::false_type
{};

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
