#include "sky/semaphore.h"

using namespace std;
using namespace sky;

semaphore::semaphore(size_t resources) :
    resource_pool(resources)
{}

bool semaphore::try_acquire()
{
    unique_lock<mutex> lock(resource_mutex, defer_lock_t());
    if (!lock.try_lock()) return false;
    if (resource_pool == 0) return false;
    resource_pool -= 1;
    return true;
}

void semaphore::acquire()
{
    unique_lock<mutex> lock(resource_mutex);
    while (resource_pool == 0)
        resource_available.wait(lock);
    resource_pool -= 1;
}

void semaphore::P()
{
    acquire();
}

void semaphore::release()
{
    lock_guard<mutex> lock(resource_mutex);
    resource_pool += 1;
    resource_available.notify_one();
}

void semaphore::V()
{
    release();
}


