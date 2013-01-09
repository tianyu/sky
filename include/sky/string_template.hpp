#ifndef STRING_TEMPLATE_HPP
#define STRING_TEMPLATE_HPP

#include <type_traits>

namespace sky {

template <char const... CHARs>
struct STR {};

template<char const S[], char const...CHARs>
struct __MAKE_STR
{
    enum {len = sizeof(S)};

    typedef
    typename std::conditional<
        (len > 0),
        typename __MAKE_STR<(char const[len-1])(S+1), CHARs..., (char const)(S[0])>::type,
        STR<CHARs...>
    >::type type;
};

template<char const S[]>
struct MAKE_STR
{
    typedef typename __MAKE_STR<S>::type type;
};

} // namespace sky

#endif // STRING_TEMPLATE_HPP
