#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>

#include "sky/tuple.hpp"
#include "sky/type_list.hpp"

#include "sky/array.hpp"

namespace  {

namespace helper {

template<std::size_t... Vals>
static std::array<int, sizeof...(Vals)>
make_array(sky::index_list<Vals...>)
{
    return std::array<int, sizeof...(Vals)>{{ Vals... }};
}

template<std::size_t... Dims>
struct rows;

template<>
struct rows<>
{
    using type = int&;
    using const_type = int const&;
    enum { num = 1 };
    enum { size = 1 };
};

template<std::size_t First>
struct rows<First>
{
    using type = int&;
    using const_type = int const&;
    enum { num = First };
    enum { size = 1 };
};

template<std::size_t First, std::size_t... Rest>
struct rows<First, Rest...>
{
    using type = sky::array<int, Rest...> &;
    using const_type = sky::array<int, Rest...> const&;
    enum { num = First };
    enum { size = sky::product<sky::index_list<Rest...>>::value };
};

} // namespace helper

template<std::size_t... Ns> struct Param
{
    using value_type = int;
    using array_type = sky::array<int, Ns...>;

    using row_type = typename helper::rows<Ns...>::type;
    using const_row_type = typename helper::rows<Ns...>::const_type;

    using coordinate_type = std::tuple<decltype(Ns)...>;

    static const std::size_t size = sky::product<sky::index_list<Ns...>>::value;

    static const std::size_t num_rows = helper::rows<Ns...>::num;
    static const std::size_t row_size = helper::rows<Ns...>::size;

    static const bool empty = (size == 0);

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
                    std::move(helper::make_array(values_t())));
    }

    static coordinate_type first_coordinate() {
        return coordinate_type {};
    }

    template<std::size_t Dim = sizeof...(Ns)>
    static void increment(coordinate_type &t)
    {
        if (Dim == 0) return;
        enum { I = Dim? Dim-1 : 0 }; // If Dim = 0, don't try to compile things
                                     // like std::get<0-1>(t);
        auto &val = std::get<I>(t);
        auto max = std::get<I>(coordinate_type{Ns...});
        if (++val < max) return;
        val = 0;
        increment<I>(t);
    }

    template<std::size_t... Is>
    static int &invoke_at(array_type &array, coordinate_type args,
                   sky::index_list<Is...>)
    {
        return array.at(std::get<Is>(args)...);
    }

    static int &invoke_at(array_type &array, coordinate_type args)
    {
        using indexes = typename sky::index_range<0, sizeof...(Ns)>::type;
        return invoke_at(array, args, indexes());
    }

};

template<typename T>
constexpr bool is_const(T &) { return false; }

template<typename T>
constexpr bool is_const(T const&) { return true; }

} // namespace

#endif // COMMON_HPP
