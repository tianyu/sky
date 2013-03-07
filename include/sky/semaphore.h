#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <atomic>

/**
 * @brief A counting semaphore.
 *
 * Semaphores are synchronization constructs.
 * Conceptually, a counting semaphore represents a
 * pool of resources. Releasing causes a resource
 * to be added to the pool, while acquiring causes
 * a resource to be removed from the pool.
 */
class semaphore
{
public:
    semaphore();

    explicit semaphore(unsigned int desired);

    semaphore(const semaphore&) = delete;
    semaphore &operator =(const semaphore&) = delete;

    bool try_acquire();
    void acquire();
    void P();

    void release();
    void V();

    bool is_lock_free();

private:
    std::atomic<unsigned int> counter;
    std::condition_variable available;
};

#endif // SEMAPHORE_H
