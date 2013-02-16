#ifndef ATOMIC_HPP
#define ATOMIC_HPP

#include <atomic>
#include <utility>
#include <type_traits>

namespace sky {

template<typename T>
/**
 * @brief An efficient atomic counter.
 *
 * An atomic counter is a special type of atomic
 * variable designed to be used only for counting.
 * In particular, multiple "writer" threads are
 * allowed to blindly increment/decrement the
 * value of the variable. "Reader" threads will only
 * see a consistent view of the counter after all
 * writer threads are finished writing.
 */
class atomic_counter
{
    static_assert(std::is_integral<T>::value &&
                  !std::is_same<T, bool>::value,
                  "T must be a non-bool, integral type.");

public:

    /**
     * @brief Creates an atomic counter with the
     * default value of the counter type.
     */
    atomic_counter();

    /**
     * @brief Creates an atomic counter with some
     * initial value.
     *
     * @param val Arguments for the initial value
     *        of the counter.
     */
    atomic_counter(T val);

    /**
     * @brief Post-increment.
     */
    void operator ++() const;

    /**
     * @brief Pre-increment (same as post-increment).
     */
    void operator ++(int) const;

    /**
     * @brief Increment by value.
     * @param val The value to increment by.
     */
    void operator +=(T val) const;

    /**
     * @brief Post-decrement.
     */
    void operator --() const;

    /**
     * @brief Pre-decrement. Same as post-decrement).
     */
    void operator --(int) const;

    /**
     * @brief Decrement by value.
     * @param val The vlaue to decrement by.
     */
    void operator -=(T val) const;

    /**
     * @brief Access the value of the counter.
     * @return The value of the counter.
     */
    T load();

    /**
     * @brief The value of the counter.
     */
    operator T();

private:
    mutable std::atomic<T> value;
};

template<typename T>
atomic_counter<T>::
atomic_counter() :
    value()
{}

template<typename T>
atomic_counter<T>::
atomic_counter(T val) :
    value(val)
{}

template<typename T>
void
atomic_counter<T>::
operator ++() const
{
    operator +=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator ++(int) const
{
    operator +=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator +=(T val) const
{
    value.fetch_add(val, std::memory_order_relaxed);
}

template<typename T>
void
atomic_counter<T>::
operator --() const
{
    operator -=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator --(int) const
{
    operator -=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator -=(T val) const
{
    value.fetch_sub(val, std::memory_order_relaxed);
}

template<typename T>
T
atomic_counter<T>::
load()
{
    return value.load(std::memory_order_relaxed);
}

template<typename T>
atomic_counter<T>::
operator T()
{
    return load();
}

} // namespace sky

#endif // ATOMIC_HPP
