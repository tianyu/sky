#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>

#include "sky/type_list.hpp"

#include "sky/array.hpp"

namespace  {

template<std::size_t... Ns> struct Param
{
private:
    template<std::size_t... Vals>
    static std::array<int, sizeof...(Vals)>
    make_array_helper(sky::index_list<Vals...>)
    {
        return std::array<int, sizeof...(Vals)>{{ Vals... }};
    }

public:

    using value_type = int;
    using array_type = sky::array<int, Ns...>;

    static const std::size_t size = sky::product<sky::index_list<Ns...>>::value;

    static const bool is_empty = (size == 0);

    static const int front = 1;

    static const int back = size;

    static int const* begin_of(array_type const&array)
    {
        return reinterpret_cast<int const*>(&array);
    }

    static int const* end_of(array_type const&array)
    {
        return begin_of(array) + size;
    }

    static int const* rbegin_of(array_type const&array)
    {
        return end_of(array) - 1;
    }

    static int const* rend_of(array_type const&array)
    {
        return begin_of(array) - 1;
    }

    static array_type make_array()
    {
        using values_t = typename sky::index_range<1, size + 1>::type;
        // This reinterpret cast should work since both sky::array and
        // std::array should have the same layout.
        return reinterpret_cast<array_type&&>(
                    std::move(make_array_helper(values_t())));
    }

};

template<typename T>
constexpr bool is_const(T &) { return false; }

template<typename T>
constexpr bool is_const(T const&) { return true; }

} // namespace

#endif // COMMON_HPP
