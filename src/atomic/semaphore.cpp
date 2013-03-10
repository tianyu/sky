#include "sky/semaphore.h"

using namespace std;
using namespace sky;

semaphore::semaphore(size_t resources) :
    counter(resources)
{}

bool semaphore::try_acquire()
{
    unique_lock<mutex> lock(counter_mutex, defer_lock_t());
    if (!lock.try_lock()) return false;
    if (counter == 0) return false;
    counter -= 1;
    return true;
}

void semaphore::acquire()
{
    unique_lock<mutex> lock(counter_mutex);
    while (counter == 0)
        available.wait(lock);
    counter -= 1;
}

void semaphore::P()
{
    acquire();
}

void semaphore::release()
{
    lock_guard<mutex> lock(counter_mutex);
    counter += 1;
    available.notify_one();
}

void semaphore::V()
{
    release();
}


