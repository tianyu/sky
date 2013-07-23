#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>

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
 *
 * ## Tuple Features
 *
 * Any array that has more than 0 dimensions is also considered a tuple.
 * Specifically, a 1-dimensional array, `array<T, N>`, is a tuple of `N`
 * elements of type `T`. And an m-dimensional array,
 * `array<T, N1, N2, ..., Nm>`, is a tuple of `N1` elements of type
 * `array<T, N2, ..., Nm>`.
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

    /**
     * Returns the number of elements in this array.
     *
     * I.e. `std::distance(begin(),end());`
     *
     * @return The number of elements in this array.
     */
    constexpr size_type size() const noexcept;

    /**
     * Returns the maximum number of elements the container is able to hold due
     * to library or implementation limitations.
     *
     * This reflects the theoretical limit of the array.
     * At runtime, the array may be limited to a value smaller than `max_size()`
     * by the amount of memory available.
     *
     * @return The maximum number of elements.
     */
    constexpr size_type max_size() const noexcept;

    /**
     * Checks whether the array has no elements.
     *
     * I.e. whether `begin() == end()`.
     *
     * @return `true` if and only if the array contains no elements.
     */
    constexpr bool empty() const noexcept;

    /**
     * Gets the n'th element of the array.
     *
     * No bounds checking is performed.
     *
     * In general, the n'th element of an m-dimensional array,
     * `array<T, N1, N2, ..., Nm>`, is the n'th (m-1)-dimensional array of type
     * `array<T, N2, ..., Nm>`.
     * So note that in this case, the type of the return value is not
     * `reference`.
     *
     * In the base case, 0-dimensional arrays do not implement this operator.
     * Instead they are implicitly convertible to the type they contain.
     * This is to make the grammar for accessing elements consistent:
     *
     *     array2[i1][i2]; // Gets an element of a 2-dimensional array.
     *     array1[i1];     // Gets an element of a 1-dimensional array.
     *     array0;         // Gets the element of a 0-dimensional array.
     *
     * @return Reference to the requested element.
     */
    reference operator[](size_type n) noexcept;
    const_reference operator[](size_type n)
        const noexcept; ///< @copydoc operator[]()

    /**
     * Gets an element of the array.
     *
     * Bounds checking is performed.
     *
     * Writing `array.at(i1, i2, ..., im);` is equivalent to
     * `array[i1][i2]...[im];` but with bounds checking.
     * In particular:
     *
     * - Each index must be of type `size_type`.
     * - There may be at most `m` indicies, where `m` is the number of
     *   dimensions.
     *
     * Calling `array.at()` with no indexes returns a reference to the array
     * itself.
     * Note that if the array is 0-dimensional, then `array.at()` will simply
     * return the value it contains.
     *
     * @throws std::out_of_range If one of the indicies are out of bounds.
     * @return Reference to the requested element.
     * @see operator[]()
     */
    template<typename... Is>
    reference at(Is&&... is);
    template<typename... Is>
    constexpr const_reference at(Is&&... is) const; ///< @copydoc at()

    /**
     * Returns a reference to the first element of the array.
     *
     * Calling `front()` on an empty array is undefined.
     *
     * @return Reference to the first element.
     */
    reference front() noexcept;
    constexpr const_reference front() const noexcept; ///< @copydoc front()

    /**
     * Returns a reference to the last element of the array.
     *
     * Calling `back()` on an empty array is undefined.
     *
     * @return Reference to the last element.
     */
    reference back() noexcept;
    constexpr const_reference back() const noexcept; ///< @copydoc back()

    /**
     * Assigns the given value to all elements of the array.
     *
     * The complexity of this operation is linear in the size of the array.
     *
     * @param value The value to fill the array with.
     */
    void fill(value_type const& value);

    /**
     * Exchanges the contents of this array with those of `other`.
     *
     * This operation does not cause iterators and references to associate with
     * the `other` array.
     *
     * @param other The other array to swap contents with.
     */
    void swap(array &other)
    noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())));

};

/**
 * Specialization of the std::swap algorithm.
 *
 * Exchanges the contents if `one` with those of `two`.
 *
 * The proper way to access this function is to use Argument Dependent Lookup
 * (ADL), in the following way:
 *
 *     using std::swap;
 *     swap(one, two);
 *
 * @param one The first array.
 * @param two The second array, to swap with the first.
 * @relates sky::array
 * @see sky::array::swap(array&)
 */
template<typename T, std::size_t... Ns>
void swap(sky::array<T, Ns...> &one, sky::array<T, Ns...> &two)
noexcept(noexcept(one.swap(two)));

} // namespace sky

namespace std {

template<typename>
class tuple_size;

/**
 * Provides access to the number of elements in a sky::array as a compile-time
 * constant expression.
 *
 * Note that this facility is not provided for 0-dimensional arrays, `array<T>`,
 * since they are not considered tuples.
 *
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
class tuple_size<sky::array<T, Ns...>>;

template<std::size_t, typename>
struct tuple_element;

/**
 * Provides the compile-time indexed access to the type of the elements of the
 * std::array.
 *
 * Note that the type of every element in a std::array is the same.
 * Specifically, the type of any element of an m-dimensional array,
 * `array<T, N1, N2, ..., Nm>` is `array<T, N2, ..., Nm>`.
 *
 * In the base case, the type of any element in a one dimensional array,
 * `array<T, N>`, is `T`.
 *
 * Note that this facility is not provided for 0-dimensional arrays, `array<T>`,
 * since they are not considered tuples.
 *
 * @relates sky::array
 */
template<std::size_t I, typename T, std::size_t... Ns>
struct tuple_element<I, sky::array<T, Ns...>>;

} // namespace std

namespace sky {

/**
 * Extracts the I'th element from the array.
 *
 * I must be an integer in the range '[0, N)', where N is the size of the array
 * as defined by `std::tuple_size`.
 * This is enforced at compile time as opposed to sky::array::operator[]() or
 * sky::array::at().
 *
 * Since this function is not provided in the standard namespace, it should be
 * accessed via Argument Dependent Lookup, for example:
 *
 *     using std::get;
 *     get<1>(array);
 *
 * Note that this facility is not provided for 0-dimensional arrays, `array<T>`,
 * since they are not considered tuples.
 *
 * @relates sky::array
 * @param a The array to get an element from.
 * @return The I'th element of the array.
 */
template<std::size_t I, typename T, std::size_t... Ns>
typename std::tuple_element<I, sky::array<T, Ns...>>::type &
get(sky::array<T, Ns...> &a) noexcept;

/**
 * Performs a lexicographical comparison between the two arrays.
 * @param one The first array to compare.
 * @param two The second array to compare.
 * @return true if and only of `one` is lexicographically equal to `two`.
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
bool operator ==(sky::array<T, Ns...> const&one,
                 sky::array<T, Ns...> const&two)
{ return std::equal(one.begin(), one.end(), two.begin()); }

/**
 * Performs a lexicographical comparison between the two arrays.
 * @param one The first array to compare.
 * @param two The second array to compare.
 * @return true if and only of `one` is not lexicographically equal to `two`.
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
bool operator !=(sky::array<T, Ns...> const&one,
                 sky::array<T, Ns...> const&two)
{ return !(one == two); }

/**
 * Performs a lexicographical comparison between the two arrays.
 * @param one The first array to compare.
 * @param two The second array to compare.
 * @return true if and only of `one` is lexicographically before `two`.
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
bool operator < (sky::array<T, Ns...> const&one,
                 sky::array<T, Ns...> const&two)
{
    return std::lexicographical_compare(one.begin(), one.end(),
                                        two.begin(), two.end());
}

/**
 * Performs a lexicographical comparison between the two arrays.
 * @param one The first array to compare.
 * @param two The second array to compare.
 * @return true if and only of `one` is lexicographically after `two`.
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
bool operator > (sky::array<T, Ns...> const&one,
                 sky::array<T, Ns...> const&two)
{ return two < one; }

/**
 * Performs a lexicographical comparison between the two arrays.
 * @param one The first array to compare.
 * @param two The second array to compare.
 * @return true if and only of `one` is not lexicographically after `two`.
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
bool operator <=(sky::array<T, Ns...> const&one,
                 sky::array<T, Ns...> const&two)
{ return !(one > two); }

/**
 * Performs a lexicographical comparison between the two arrays.
 * @param one The first array to compare.
 * @param two The second array to compare.
 * @return true if and only of `one` is not lexicographically before `two`.
 * @relates sky::array
 */
template<typename T, std::size_t... Ns>
bool operator >=(sky::array<T, Ns...> const&one,
                 sky::array<T, Ns...> const&two)
{ return !(one < two); }

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

    constexpr const_pointer data() const noexcept
    { return const_pointer(&_elem); }

    iterator begin() noexcept
    { return iterator(data()); }

    constexpr const_iterator begin() const noexcept
    { return const_iterator(data()); }

    constexpr const_iterator cbegin() const noexcept
    { return const_iterator(data()); }

    iterator end() noexcept
    { return iterator(data() + 1); }

    constexpr const_iterator end() const noexcept
    { return const_iterator(data() + 1); }

    constexpr const_iterator cend() const noexcept
    { return const_iterator(data() + 1); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    constexpr const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    constexpr const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    constexpr const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr size_type size() const noexcept
    { return size_type(1); }

    constexpr size_type max_size() const noexcept
    { return size(); }

    constexpr bool empty() const noexcept
    { return size() == 0; }

    /*
      The accessors for a 0-dimensional array are different than in the general
      case. This is due to the natural extension of the grammar used access
      elements.

      Without bounds checking:
      array2[i][k]; // 2-dimensional
      array1[i];    // 1-dimensional
      array0;       // 0-dimensional

      With bounds checking:
      array2.at(i, k);  // 2-dimensional
      array1.at(i);     // 1-dimensional
      array0.at();      // 0-dimensional

      Hence the 0-dimensional array does not implement operator[]. Instead, it
      is implicitly convertible to a value of the type it contains. This is
      consistent with the concept of a 0-dimensional matrix.
    */

    operator reference() noexcept
    { return _elem; }

    constexpr operator const_reference() const noexcept
    { return _elem; }

    reference at() noexcept
    { return _elem; }

    constexpr const_reference at() const noexcept
    { return _elem; }

    reference front() noexcept
    { return _elem; }

    constexpr const_reference front() const noexcept
    { return _elem; }

    reference back() noexcept
    { return _elem; }

    constexpr const_reference back() const noexcept
    { return _elem; }

    void fill(value_type const& value)
    { _elem = value; }

    void swap(array &other)
    noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())))
    { std::swap(_elem, other._elem); }

};

namespace _ {

/*
 * See this bug: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53248
 * Which also affected this code as well.
 */

template<typename value_type, std::size_t N>
struct array_traits
{
    using data_type = value_type[N];

    static constexpr value_type *
    get_data(data_type &d) noexcept
    { return &d[0]; }

    static constexpr value_type const*
    get_data(data_type const&d) noexcept
    { return &d[0]; }

    static constexpr value_type &
    get_elem(data_type &d, std::size_t n) noexcept
    { return d[n]; }

    static constexpr value_type const&
    get_elem(data_type const&d, std::size_t n) noexcept
    { return d[n]; }

};

template<typename value_type>
struct array_traits<value_type, 0>
{
    struct data_type {};

    static constexpr value_type *
    get_data(data_type &d) noexcept
    { return static_cast<value_type*>(nullptr); }

    static constexpr value_type const*
    get_data(data_type const&d) noexcept
    { return static_cast<value_type const*>(nullptr); }

    static constexpr value_type &
    get_elem(data_type &d, std::size_t n) noexcept
    { return *static_cast<value_type*>(nullptr); }

    static constexpr value_type const&
    get_elem(data_type const&d, std::size_t n) noexcept
    { return *static_cast<value_type const*>(nullptr); }

};

} // namespace _

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

    using array_traits = _::array_traits<value_type, N>;
    typename array_traits::data_type _elems;

    pointer data() noexcept
    { return array_traits::get_data(_elems); }

    constexpr const_pointer data() const noexcept
    { return array_traits::get_data(_elems); }

    iterator begin() noexcept
    { return iterator(data()); }

    constexpr const_iterator begin() const noexcept
    { return const_iterator(data()); }

    constexpr const_iterator cbegin() const noexcept
    { return const_iterator(data()); }

    iterator end() noexcept
    { return iterator(data() + N); }

    constexpr const_iterator end() const noexcept
    { return const_iterator(data() + N); }

    constexpr const_iterator cend() const noexcept
    { return const_iterator(data() + N); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    constexpr const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    constexpr const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    constexpr const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr size_type size() const noexcept
    { return size_type(N); }

    constexpr size_type max_size() const noexcept
    { return size(); }

    constexpr bool empty() const noexcept
    { return size() == 0; }

    reference operator[](size_type n) noexcept
    { return _elems[n]; }

    constexpr const_reference operator[](size_type n) const noexcept
    { return _elems[n]; }

    array<T, N> &at() noexcept
    { return *this; }

    constexpr array<T, N> const&at() const noexcept
    { return *this; }

    reference at(size_type n)
    {
        if (n >= N) throw std::out_of_range("array::at");
        return _elems[n];
    }

    constexpr const_reference at(size_type n) const
    {
        return (n < N)? _elems[n] :
                        throw std::out_of_range("array::at"),
                        _elems[n];
    }

    reference front() noexcept
    { return *begin(); }

    constexpr const_reference front() const noexcept
    { return *begin(); }

    reference back() noexcept
    { return N? *(end()-1) : *end(); }

    constexpr const_reference back() const noexcept
    { return N? *(end()-1) : *end(); }

    void fill(value_type const& value)
    { std::fill(begin(), end(), value); }

    void swap(array &other)
    noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())))
    { std::swap_ranges(begin(), end(), other.begin()); }

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

    row_type _rows[N1? N1 : 1];

    pointer data() noexcept
    { return _rows[0].data(); }

    constexpr const_pointer data() const noexcept
    { return _rows[0].data(); }

    iterator begin() noexcept
    { return _rows[0].begin(); }

    constexpr const_iterator begin() const noexcept
    { return _rows[0].begin(); }

    constexpr const_iterator cbegin() const noexcept
    { return _rows[0].begin(); }

    iterator end() noexcept
    { return _rows[N1-1].end(); }

    constexpr const_iterator end() const noexcept
    { return _rows[N1-1].end(); }

    constexpr const_iterator cend() const noexcept
    { return _rows[N1-1].end(); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    constexpr const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    constexpr const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    constexpr const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr size_type size() const noexcept
    { return N1 * _rows[0].size(); }

    constexpr size_type max_size() const noexcept
    { return size(); }

    constexpr bool empty() const noexcept
    { return size() == 0; }

    row_type &operator[](size_type n) noexcept
    { return _rows[n]; }

    constexpr row_type const&operator[](size_type n) const noexcept
    { return _rows[n]; }

    array<T, N1, Ns...> &at() noexcept
    { return *this; }

    constexpr array<T, N1, Ns...> const&at() const noexcept
    { return *this; }

    template<typename... Is>
    auto at(size_type i, Is&&... is)
        -> decltype(_rows[i].at(is...))
    {
        if (i >= N1) throw std::out_of_range("array::at");
        return _rows[i].at(std::forward<Is>(is)...);
    }

    template<typename... Is>
    constexpr auto at(size_type i, Is&&... is) const
        -> decltype(_rows[i].at(is...))
    {
        return (i < N1)? _rows[i].at(std::forward<Is>(is)...) :
                         throw std::out_of_range("array::at"),
                         _rows[i].at(std::forward<Is>(is)...);
    }

    reference front() noexcept
    { return *begin(); }

    constexpr const_reference front() const noexcept
    { return *begin(); }

    reference back() noexcept
    { return empty()? *end() : *(end()-1); }

    constexpr const_reference back() const noexcept
    { return empty()? *end() : *(end()-1); }

    void fill(value_type const& value)
    { std::fill(begin(), end(), value); }

    void swap(array &other)
    noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())))
    { std::swap_ranges(begin(), end(), other.begin()); }

};

// Specialized algorithms //

template<typename T, std::size_t... Ns>
inline void swap(sky::array<T, Ns...> &a1, sky::array<T, Ns...> &a2)
noexcept(noexcept(a1.swap(a2)))
{ a1.swap(a2); }

} // namespace sky

namespace std {

template<typename T, std::size_t N, std::size_t... Ns>
class tuple_size<sky::array<T, N, Ns...>> :
        public std::integral_constant<std::size_t, N>
{};

template<std::size_t I, typename T, std::size_t N>
struct tuple_element<I, sky::array<T, N>>
{ using type = T; };

template<std::size_t I, typename T, std::size_t N, std::size_t... Ns>
struct tuple_element<I, sky::array<T, N, Ns...>>
{ using type = sky::array<T, Ns...>; };

} // namespace std

namespace sky {

template<std::size_t I, typename T, std::size_t... Ns>
typename std::tuple_element<I, sky::array<T, Ns...>>::type &
get(sky::array<T, Ns...> &a) noexcept
{
    enum { N = std::tuple_size<sky::array<T, Ns...>>::value };
    static_assert(I < N, "get<I>: Index I is out of range.");
    return a[I];
}

template<std::size_t I, typename T, std::size_t... Ns>
typename std::tuple_element<I, sky::array<T, Ns...>>::type &&
get(sky::array<T, Ns...> &&a) noexcept
{
    enum { N = std::tuple_size<sky::array<T, Ns...>>::value };
    static_assert(I < N, "get<I>: Index I is out of range.");
    return std::move(a[I]);
}

template<std::size_t I, typename T, std::size_t... Ns>
constexpr
typename std::tuple_element<I, sky::array<T, Ns...>>::type const&
get(sky::array<T, Ns...> const&a) noexcept
{
    enum { N = std::tuple_size<sky::array<T, Ns...>>::value };
    static_assert(I < N, "get<I>: Index I is out of range.");
    return a[I];
}

} // namespace sky

#endif // ARRAY_HPP
