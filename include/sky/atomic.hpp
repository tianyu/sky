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
     * @brief Creates an atomic counter.
     *
     * No initialization takes place other than
     * zero initialization of static and thread-local
     * objects.
     */
    atomic_counter() noexcept = default;

    /**
     * @brief Creates an atomic counter with some
     * initial value.
     *
     * The initialization is not atomic.
     *
     * @param val Arguments for the initial value
     *        of the counter.
     */
    constexpr atomic_counter(T val) noexcept;

    // Atomic counters are not copy constructible.
    atomic_counter(atomic_counter const&) = delete;

    // Atomic counters are not copy assignable.
    atomic_counter &operator =(atomic_counter const&) = delete;
    volatile atomic_counter &operator =(atomic_counter const&) volatile = delete;

    /// @{
    /**
     * @brief Checks whether the atomic operations
     * on the object are lock-free.
     *
     * @return true iff the atomic operations are
     * lock-free.
     */
    bool is_lock_free() const noexcept;
    bool is_lock_free() const volatile noexcept;
    /// @}

    /// @{
    /**
     * @brief Increment.
     */
    void operator ++() const noexcept;
    void operator ++() const volatile noexcept;
    void operator ++(int) const noexcept;
    void operator ++(int) const volatile noexcept;
    /// @}

    /// @{
    /**
     * @brief Increment by value.
     * @param val The value to increment by.
     */
    void operator +=(T val) const noexcept;
    void operator +=(T val) const volatile noexcept;
    /// @}

    /// @{
    /**
     * @brief Post-decrement.
     */
    void operator --() const noexcept;
    void operator --() const volatile noexcept;
    void operator --(int) const noexcept;
    void operator --(int) const volatile noexcept;
    /// @}

    /// @{
    /**
     * @brief Decrement by value.
     * @param val The vlaue to decrement by.
     */
    void operator -=(T val) const noexcept;
    void operator -=(T val) const volatile noexcept;
    /// @}

    /// @{
    /**
     * @brief Access the value of the counter.
     * @return The value of the counter.
     */
    T load() noexcept;
    T load() volatile noexcept;
    /// @}

    /// @{
    /**
     * @brief The value of the counter.
     */
    operator T() noexcept;
    operator T() volatile noexcept;
    /// @}

private:
    mutable std::atomic<T> value;
};

template<typename T>
constexpr
atomic_counter<T>::
atomic_counter(T val) noexcept :
    value(val)
{}

template<typename T>
void
atomic_counter<T>::
operator ++() const noexcept
{
    operator +=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator ++(int) const noexcept
{
    operator +=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator +=(T val) const noexcept
{
    value.fetch_add(val, std::memory_order_relaxed);
}

template<typename T>
void
atomic_counter<T>::
operator --() const noexcept
{
    operator -=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator --(int) const noexcept
{
    operator -=(T(1));
}

template<typename T>
void
atomic_counter<T>::
operator -=(T val) const noexcept
{
    value.fetch_sub(val, std::memory_order_relaxed);
}

template<typename T>
T
atomic_counter<T>::
load() noexcept
{
    return value.load(std::memory_order_relaxed);
}

template<typename T>
atomic_counter<T>::
operator T() noexcept
{
    return load();
}

} // namespace sky

#endif // ATOMIC_HPP
