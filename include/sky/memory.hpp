#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <memory>
#include <utility>
#include <type_traits>

namespace sky {

/**
 * @brief Constructs an object of type T and wraps it in
 * a @ref std::unique_ptr.
 * @param args The arguments to construct T with.
 * @return A @ref std::unique_ptr of type T.
 */
template<typename T, typename... Args>
std::unique_ptr<T>
make_unique(Args&&... args)

namespace _ {

template<typename T, typename... Args>
std::unique_ptr<T>
__make_unique(std::false_type, Args&&... args)
{
    return std::unique_ptr<T>(
                new T(std::forward<Args>(args)...));
}

template<typename A, typename... Args>
std::unique_ptr<A>
__make_unique(std::true_type, Args&&... args)
{
    static_assert(std::extent<A>::value == 0,
            "make_unique<T[N]>() is forbidden. "
            "Use make_unique<T[]>() instead.");

    typedef typename std::remove_extent<A>::type T;
    return std::unique_ptr<A>(
                new T[sizeof...(Args)]{
                    std::forward<Args>(args)...});
}

} // namespace _

template<typename T, typename... Args>
std::unique_ptr<T>
make_unique(Args&&... args)
{
    return _::__make_unique<T>(
                std::is_array<T>(),
                std::forward<Args>(args)...);
}

} // namespace sky

#endif // MEMORY_HPP
