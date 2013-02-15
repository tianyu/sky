#ifndef ATOMIC_HPP
#define ATOMIC_HPP

#include <atomic>

namespace sky {

template<typename T>
/**
 * @brief An efficient atomic counter.
 *
 * An atomic counter is a special type of atomic
 * variable designed to be used only for counting.
 * In particular, multiple "writer" threads are
 * allowed to blindly increment/decrement the
 * value of the variable. "Reader" will only see
 * a consistent view of the counter after all
 * writer threads are finished writing.
 */
class atomic_counter
{
public:

    /**
     * @brief Creates an atomic counter with the
     * default value of the counter type.
     */
    atomic_counter();

    template <typename... Args>
    /**
     * @brief Creates an atomic counter and
     * emplaces the initial value of the counter.
     *
     * @param args Arguments for the initial value
     *        of the counter.
     */
    atomic_counter(Args... args);

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
     * @param t The value to increment by.
     */
    void operator +=(T const&t) const;

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
     * @param t The vlaue to decrement by.
     */
    void operator -=(T const&t) const;

    /**
     * @brief Access the value of the counter.
     * @return The value of the counter.
     */
    T const& get();

    /**
     * @brief The value of the counter.
     */
    operator T const&();

private:
    mutable std::atomic<T> value;
};

} // namespace sky

#endif // ATOMIC_HPP
