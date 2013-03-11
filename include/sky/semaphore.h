#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

namespace sky {

/**
 * @brief A counting semaphore.
 *
 * Semaphores are synchronization constructs. Conceptually, a counting semaphore
 * represents a pool of resources. Releasing causes a resource to be added to
 * the pool, while acquiring causes a resource to be removed from the pool. If
 * there are no resources to acquire, then the thread that called acquire() will
 * block until a resource has been released. Alternatively, a thread may call
 * the non-blocking try_acquire().
 */
class semaphore
{
public:
    /**
     * @brief Create a semaphore with an initial number of resources.
     *
     * @param resources The number of resources available at the beginning.
     */
    explicit semaphore(int resources = 1);

    semaphore(unsigned int) = delete;
    semaphore(long) = delete;

    semaphore(const semaphore&) = delete;
    semaphore &operator =(const semaphore&) = delete;

    /**
     * @brief Try to acquire a resource from the semaphore
     * @return true iff a resource was acquired.
     */
    bool try_acquire();

    /** @{
     * @brief Acquires a resource from the semaphore.
     *
     * This function blocks until there is a resource available to be acquired.
     */
    void acquire();
    void P();
    /// @}

    /** @{
     * @brief Releases a resource back into the semaphore.
     *
     * This is a non-blocking operation.
     */
    void release();
    void V();
    /// @}

private:
    std::mutex resource_mutex;
    std::condition_variable resource_available;
    int resource_pool;
    unsigned int released;
};

} // namespace sky

#endif // SEMAPHORE_H
