#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>
#include <iterator>

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
 * Note that A 0-dimensional array, `array<T>`, contains exactly one element.
 *
 * `array` is an aggregate (it has no user-defined constructors and no private
 * or protected members), which allows it to use aggregate-initialization.
 */
template<typename T, std::size_t... Ns>
struct array;

template<typename T>
struct array<T>
{
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    value_type _elem;
};

template<typename T, std::size_t N>
struct array<T, N>
{
    using row_type = sky::array<T>;
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

    value_type _elems[N];
};

template<typename T, std::size_t N1, std::size_t... Ns>
struct array<T, N1, Ns...>
{
    using row_type = sky::array<T, Ns...>;
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

    row_type _rows[N1];

};

} // namespace sky

#endif // ARRAY_HPP
