#include "sky/semaphore.h"

using namespace std;
using namespace sky;

semaphore::semaphore(int resources) :
    resource_pool(resources),
    released(resources < 0? resources : 0)
{}

bool semaphore::try_acquire()
{
    unique_lock<mutex> lock(resource_mutex, defer_lock_t());
    if (!lock.try_lock()) return false;
    if (resource_pool < 1) return false;
    resource_pool -= 1;
    return true;
}

bool semaphore::try_P()
{
    return semaphore::try_acquire();
}

void semaphore::acquire()
{
    unique_lock<mutex> lock(resource_mutex);

    /*
     * If there are available resources, acquire one.
     * Otherwise, wait for a resource to be released and
     * acquire the released resource immediately.
     */

    if ((resource_pool--) > 0) return;
    do {
        resource_available.wait(lock);
    } while (released < 1);
    --released;
}

void semaphore::P()
{
    acquire();
}

void semaphore::release()
{
    lock_guard<mutex> lock(resource_mutex);

    /*
     * If there are no waiters, just release the resource into
     * the pool. Otherwise, notify a waiting thread of the
     * released resource.
     */

    if ((resource_pool++) > -1) return;
    ++released;
    if (released > 0)
        resource_available.notify_one();
}

void semaphore::V()
{
    release();
}


