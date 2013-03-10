#include "sky/semaphore.h"

using namespace std;
using namespace sky;

semaphore::semaphore(size_t resources) :
    counter(resources)
{}

bool semaphore::try_acquire() {
    unique_lock<mutex> lock(counter_mutex, defer_lock_t());
    if (!lock.try_lock()) return false;
    if (counter == 0) return false;
    counter -= 1;
    return true;
}


