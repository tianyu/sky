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

#undef STD_BINARY_PREDICATE

} // namespace predicate

template<typename Predicate>
struct relate
{
    template<typename T, typename U>
    struct op : public Predicate::template op<T,U>
    {};



};

} // namespace sky

#endif // TYPE_TRAITS_HPP
