#ifndef TYPE_LIST_HPP
#define TYPE_LIST_HPP

namespace sky {

template <typename... Types>
class type_list
{
    template <typename First, typename... Rest>
    struct non_empty_list
    {
        typedef First first;
        typedef type_list<Rest...> rest;
    };

public:
    static const bool empty = false;
    typedef typename non_empty_list<Types...>::first first;
    typedef typename non_empty_list<Types...>::rest rest;
};

template <>
struct type_list<>
{
    static const bool empty = true;
};

} // namespace sky

#endif // TYPE_LIST_HPP
