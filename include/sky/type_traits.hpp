#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

/**
 * @defgroup type_trait Type Traits
 */

namespace sky {

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

namespace predicate {

#define STD_UNARY_PREDICATE(name) \
struct name \
{ \
    template<typename T> \
    using op = std::name<T>; \
}

STD_UNARY_PREDICATE(is_void);
STD_UNARY_PREDICATE(is_integral);
STD_UNARY_PREDICATE(is_floating_point);
STD_UNARY_PREDICATE(is_array);
STD_UNARY_PREDICATE(is_enum);
STD_UNARY_PREDICATE(is_union);
STD_UNARY_PREDICATE(is_class);
STD_UNARY_PREDICATE(is_function);
STD_UNARY_PREDICATE(is_pointer);
STD_UNARY_PREDICATE(is_lvalue_reference);
STD_UNARY_PREDICATE(is_rvalue_reference);
STD_UNARY_PREDICATE(is_member_object_pointer);
STD_UNARY_PREDICATE(is_member_function_pointer);

STD_UNARY_PREDICATE(is_fundamental);
STD_UNARY_PREDICATE(is_arithmetic);
STD_UNARY_PREDICATE(is_scalar);
STD_UNARY_PREDICATE(is_object);
STD_UNARY_PREDICATE(is_compound);
STD_UNARY_PREDICATE(is_reference);
STD_UNARY_PREDICATE(is_member_pointer);

STD_UNARY_PREDICATE(is_const);
STD_UNARY_PREDICATE(is_volatile);
STD_UNARY_PREDICATE(is_trivial);
// STD_UNARY_PREDICATE(is_trivially_copyable);
STD_UNARY_PREDICATE(is_standard_layout);
STD_UNARY_PREDICATE(is_pod);
STD_UNARY_PREDICATE(is_literal_type);
STD_UNARY_PREDICATE(is_empty);
STD_UNARY_PREDICATE(is_polymorphic);
STD_UNARY_PREDICATE(is_abstract);
STD_UNARY_PREDICATE(is_signed);
STD_UNARY_PREDICATE(is_unsigned);

STD_UNARY_PREDICATE(is_constructible);
STD_UNARY_PREDICATE(is_default_constructible);
STD_UNARY_PREDICATE(is_copy_constructible);
STD_UNARY_PREDICATE(is_move_constructible);
STD_UNARY_PREDICATE(is_copy_assignable);
STD_UNARY_PREDICATE(is_move_assignable);
STD_UNARY_PREDICATE(has_virtual_destructor);
STD_UNARY_PREDICATE(is_destructible);

#undef STD_UNARY_PREDICATE

#define STD_BINARY_PREDICATE(name) \
struct name \
{ \
    template<typename T, typename U> \
    using op = std::name<T,U>; \
}

#define STD_BINARY_REVERSE_PREDICATE(name, pred) \
struct name \
{ \
    template<typename T, typename U> \
    using op = std::pred<U,T>; \
}

STD_BINARY_PREDICATE(is_same);
STD_BINARY_PREDICATE(is_base_of);
STD_BINARY_REVERSE_PREDICATE(is_derived_from, is_base_of);
STD_BINARY_PREDICATE(is_convertible);
STD_BINARY_REVERSE_PREDICATE(is_convertible_from, is_convertible);
STD_BINARY_PREDICATE(is_assignable);
STD_BINARY_REVERSE_PREDICATE(is_assignable_from, is_assignable);

#undef STD_BINARY_PREDICATE
#undef STD_BINARY_REVERSE_PREDICATE

} // namespace predicate

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
