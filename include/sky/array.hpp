#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <array>

namespace sky {

/**
 * sky::array is a container that encapsulates multi-dimensional arrays.
 *
 * Thus struct has the same aggregate type semantics as a C-style array.
 * The size and efficienty of an m-dimensional array,
 * `array<T, N1, N2, ..., Nm>`, is equivalent to the size and efficiency of
 * `T[N1*N2*...*Nm]`.
 * The struct provides the benefits of a standard container, such as knowing its
 * own size, supporting assignment, random access iterators, etc.
 *
 * In the special case where some dimension `Ni == 0`,
 * `array.begin() == array.end()`, which is some unique value.
 * The effect of calling `front()` or `back()` on such an array is undefined.
 *
 * `array` is an aggregate (it has no user-defined constructors and no private
 * or protected members), which allows it to use aggregate-initialization.
 */
template<typename T, std::size_t... Ns>
struct array;

template<typename T, std::size_t N>
struct array<T, N> : public std::array<T, N>
{
};

template<typename T>
struct array<T> : public std::array<T, 0>
{
};

template<typename T, std::size_t N1, std::size_t... Ns>
struct array<T, N1, Ns...>
{
    using row_type = array<T, Ns...>;
    using value_type = typename row_type::value_type;
    using size_type = typename row_type::size_type;
    using difference_type = typename row_type::difference_type;
    using reference = typename row_type::reference;
    using const_reference = typename row_type::const_reference;
    using pointer = typename row_type::pointer;
    using const_pointer = typename row_type::const_pointer;
    using iterator = typename row_type::iterator;
    using const_iterator = typename row_type::const_iterator;
    using reverse_iterator = typename row_type::reverse_iterator;
    using const_reverse_iterator = typename row_type::const_reverse_iterator;
};

} // namespace sky

#endif // ARRAY_HPP
