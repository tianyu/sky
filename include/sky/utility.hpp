#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <type_traits>
#include <utility>

namespace sky {

/**
 * @brief A reference forwarding helper class.
 *
 * This class is designed specifically for use with contructors.
 *
 * Programmers and compilers can generally work together to enable optimizations
 * for argument passing. For regular functions, fully optimized argument passing
 * can be realized with relatively little effort. Constructors, however, often
 * use their arguments to initialize their class's member variables. This can
 * cause unnecessary copy or move operations.
 *
 * Several techniques are used to optimize the number of copy/move operations in
 * a constructor. However each of them have problems:
 *
 * - Variadic templates + perfect forwarding: Achieves optimality, but only
 *   works when the constructor takes a variable number of arguments.
 * - Regular templates + perfect forwarding: Handles a constant number of
 *   arguments, but can cause issues with multiple constructors that can only
 *   be solved with complex template-metaprogramming (e.g. enable_if).
 * - rvalue references + std::move: No longer depends on templates, but requires
 *   an exponential number of constructors for optimality.
 *
 * This class is therefore designed to handle the optimal passing of a constant
 * number of arguments without an exponential number of overloads. fwd objects
 * remember lvalue/rvalue-ness of the reference they are constructed with and
 * will copy or move the referenced object as appropriate.
 */
template <typename T>
class fwd
{
    static_assert(!std::is_reference<T>::value,
        "T cannot be a reference type.");

    static_assert(!std::is_const<T>::value,
        "T cannot be a const type.");

public:

    /**
     * @brief Capture l-value references to be forwarded.
     */
    fwd(T const&);

    /**
     * @brief Capture r-value references to be forwarded.
     */
    fwd(T &&);

    /**
     * @brief Move constructor
     */
    fwd(fwd &&);

    /**
     * @brief Copy constructor is disabled.
     */
    fwd(fwd const&) = delete;

    /** @{
     * @brief Assignment is disabled.
     */
    fwd &operator =(fwd const&) = delete;
    fwd &operator =(fwd &&) = delete;
    /// @}

    /**
     * @brief Convert the forwarded reference to an object.
     *
     * Moves the captured reference if it was an r-value reference.
     * Copies the captured reference otherwise.
     */
    operator T();

private:
    union {
        T const*to_copy;
        T *to_move;
    };
    const bool movable;
};

template<typename T>
fwd<T>::fwd(fwd &&f) :
    to_copy(std::move(f.to_copy)),
    movable(f.movable)
{}

template<typename T>
fwd<T>::fwd(T const&ref) :
    to_copy(&ref),
    movable(false)
{}

template<typename T>
fwd<T>::fwd(T &&ref) :
    to_move(&ref),
    movable(true)
{}

template<typename T>
fwd<T>::operator T()
{
    if (movable) return std::move(*to_move);
    return *to_copy;
}

} // namespace sky

#endif // UTILITY_HPP
