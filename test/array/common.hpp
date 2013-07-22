#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>

#include "sky/array.hpp"

#include "sky/tuple.hpp"
#include "sky/type_list.hpp"

namespace  {

namespace helper {

template<std::size_t... Vals>
static std::array<int, sizeof...(Vals)>
make_array(sky::index_list<Vals...>)
{
    return std::array<int, sizeof...(Vals)>{{ Vals... }};
}

template<typename T, std::size_t... Dims>
struct rows;

template<typename T>
struct rows<T>
{
    using type = T;
    using const_type = T const;
    enum { num = 1 };
    enum { size = 1 };
};

template<typename T, std::size_t First>
struct rows<T, First>
{
    using type = T;
    using const_type = T const;
    enum { num = First };
    enum { size = 1 };
};

template<typename T, std::size_t First, std::size_t... Rest>
struct rows<T, First, Rest...>
{
    using type = sky::array<T, Rest...>;
    using const_type = sky::array<T, Rest...> const;
    enum { num = First };
    enum { size = sky::product<sky::index_list<Rest...>>::value };
};

} // namespace helper

template<typename T, std::size_t... Ns>
struct Param
{
    using value_type = T;
    using array_type = sky::array<T, Ns...>;

    using row_type = typename helper::rows<T, Ns...>::type;
    using const_row_type = typename helper::rows<T, Ns...>::const_type;

    using coordinate_type = std::tuple<decltype(Ns)...>;

    enum { size = sky::product<sky::index_list<Ns...>>::value };
    enum { empty = (size == 0) };

    enum { num_rows = helper::rows<T, Ns...>::num };
    enum { row_size = helper::rows<T, Ns...>::size };

    static T const* begin_of(array_type const&array)
    {
        return reinterpret_cast<T const*>(&array);
    }

    static T const* end_of(array_type const&array)
    {
        return begin_of(array) + size;
    }

    static T const* rbegin_of(array_type const&array)
    {
        return end_of(array) - 1;
    }

    static T const* rend_of(array_type const&array)
    {
        return begin_of(array) - 1;
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
    static T &invoke_at(array_type &array, coordinate_type args,
                   sky::index_list<Is...>)
    {
        return array.at(std::get<Is>(args)...);
    }

    static T &invoke_at(array_type &array, coordinate_type args)
    {
        using indexes = typename sky::index_range<0, sizeof...(Ns)>::type;
        return invoke_at(array, args, indexes());
    }

    template<std::size_t... Is>
    static T const&invoke_at(array_type const&array, coordinate_type args,
                   sky::index_list<Is...>)
    {
        return array.at(std::get<Is>(args)...);
    }

    static T const&invoke_at(array_type const&array, coordinate_type args)
    {
        using indexes = typename sky::index_range<0, sizeof...(Ns)>::type;
        return invoke_at(array, args, indexes());
    }

};

template<std::size_t... Ns>
struct IntParam : public Param<int, Ns...>
{

    using typename Param<int, Ns...>::array_type;
    using Param<int, Ns...>::size;

    static array_type make_array()
    {
        using values_t = typename sky::index_range<1, size + 1>::type;
        // This reinterpret cast should work since both sky::array and
        // std::array should have the same layout.
        return reinterpret_cast<array_type&&>(
                    std::move(helper::make_array(values_t())));
    }

};

template<typename T>
constexpr bool is_const(T &) { return false; }

template<typename T>
constexpr bool is_const(T const&) { return true; }

} // namespace

#endif // COMMON_HPP
