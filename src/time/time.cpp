#include "sky/time.h"
#include <ctime>

using namespace sky;

class time time::epoch() {
    time t;
    t.s = 0;
    t.ns = 0;
    return t;
}

class time time::now()
{
    time t;
    clock_gettime(CLOCK_REALTIME, reinterpret_cast<timespec *>(&t));
    return t;
}

class time& time::operator -=(time t)
{
    s -= t.s;
    ns -= t.ns;
    return *this;
}

class time time::operator -(time t) const
{
    time result;
    result.s = s - t.s;
    result.ns = ns - t.ns;
    return result;
}

long time::millis() const
{
    return s*1000L + ns/1000000L;
}

long long time::nanos() const
{
    return s * 1000000000LL + ns;
}
