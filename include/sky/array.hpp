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
struct array
{
    /*
      This class only contains definitions and documentation.
    */

    /// The value type.
    using value_type = T;

    /// The pointer type.
    using pointer = value_type*;

    /// The pointer to constant type.
    using const_pointer = value_type const*;

    /// The reference type.
    using reference = value_type&;

    /// The reference to constant type.
    using const_reference = value_type const&;

    /// The iterator type.
    using iterator = pointer;

    /// The iterator to constant type.
    using const_iterator = const_pointer;

    /// The reverse iterator type.
    using reverse_iterator = std::reverse_iterator<iterator>;

    /// The reverse iterator to constant type.
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /// The size type.
    using size_type = std::size_t;

    /// The difference type.
    using difference_type = std::ptrdiff_t;

    /**
     * Direct access to the underlying array.
     *
     * The pointer is such that the range `[data(); data() + size()]` is always
     * a valid range, even if the container is empty.
     *
     * @return A pointer to the underlying array.
     */
    pointer data() noexcept;
    const_pointer data() const noexcept; ///< @copydoc data()

    /**
     * Returns an iterator to the first element of the array.
     *
     * @return An iterator to the first element.
     */
    iterator begin() noexcept;
    const_iterator begin() const noexcept; ///< @copydoc begin()
    const_iterator cbegin() const noexcept; ///< @copydoc begin()

    /**
     * Returns an iterator to the element following the last element of the
     * array.
     *
     * This last element acts as a placeholder; attempting to access it results
     * in undefined behavior.
     *
     * @return An iterator to the element following the last element.
     */
    iterator end() noexcept;
    const_iterator end() const noexcept; ///< @copydoc end()
    const_iterator cend() const noexcept; ///< @copydoc end()

    /**
     * Returns a reverse iterator to the first element of the reversed array.
     *
     * It corresponds to the last element of the non-reversed array.
     *
     * @return An iterator to the first element.
     */
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept; ///< @copydoc rbegin()
    const_reverse_iterator crbegin() const noexcept; ///< @copydoc rbegin()

    /**
     * Returns a reverse iterator to the element following the last element of
     * the reversed array.
     *
     * It corresponds to the element preceding the first element of the
     * non-reversed array.
     * This last element acts as a placeholder; attempting to access it results
     * in undefined behavior.
     *
     * @return An iterator to the element following the last element.
     */
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept; ///< @copydoc rend()
    const_reverse_iterator crend() const noexcept; ///< @copydoc rend()

};

/*
  The 0-dimensional base case, which contains exactly one element.

  All the member types are defined here.
  Higher dimensional arrays get their member type definitions from here.
*/
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

    pointer data() noexcept
    { return pointer(&_elem); }

    const_pointer data() const noexcept
    { return const_pointer(&_elem); }

    iterator begin() noexcept
    { return iterator(data()); }

    const_iterator begin() const noexcept
    { return const_iterator(data()); }

    const_iterator cbegin() const noexcept
    { return const_iterator(data()); }

    iterator end() noexcept
    { return iterator(data() + 1); }

    const_iterator end() const noexcept
    { return const_iterator(data() + 1); }

    const_iterator cend() const noexcept
    { return const_iterator(data() + 1); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

};

/*
  The 1-dimensional special case.

  Currently, this exists because otherwise, users would be forced to place { }
  around each element of an array, like:

  sky::array<int, 3>{{ {1}, {2}, {3} }}; // Ugly

  Instead of:

  sky::array<int, 3>{{ 1, 2, 3 }}; // Prettier

  Not adding the braces would still allow the code to compile (on some compilers
  and only for certain types), but it causes the compiler to issue a warning.

  Hopefully, when aggregate syntax is simplified in C++14, this specialization
  will no longer have to exist, allowing us to write:

  sky::array<int, 2, 3>{ {1, 2, 3}, {4, 5, 6} }; // Awesome!
*/
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

    pointer data() noexcept
    { return pointer(&_elems[0]); }

    const_pointer data() const noexcept
    { return pointer(&_elems[0]); }

    iterator begin() noexcept
    { return iterator(data()); }

    const_iterator begin() const noexcept
    { return const_iterator(data()); }

    const_iterator cbegin() const noexcept
    { return const_iterator(data()); }

    iterator end() noexcept
    { return iterator(data() + N); }

    const_iterator end() const noexcept
    { return const_iterator(data() + N); }

    const_iterator cend() const noexcept
    { return const_iterator(data() + N); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

};

/*
  The general m-dimensional case.

  An m-dimensional array with array sizes N1, N2, ..., Nm is implemented as an
  array of N1 rows, where each row is a (m-1)-dimensional array with sizes
  N2, N3, ..., Nm.
*/
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

    pointer data() noexcept
    { return _rows[0].data(); }

    const_pointer data() const noexcept
    { return _rows[0].data(); }

    iterator begin() noexcept
    { return _rows[0].begin(); }

    const_iterator begin() const noexcept
    { return _rows[0].begin(); }

    const_iterator cbegin() const noexcept
    { return _rows[0].begin(); }

    iterator end() noexcept
    { return _rows[N1-1].end(); }

    const_iterator end() const noexcept
    { return _rows[N1-1].end(); }

    const_iterator cend() const noexcept
    { return _rows[N1-1].end(); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

};

} // namespace sky

#endif // ARRAY_HPP
