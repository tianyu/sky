#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace sky {

class timer
{
    typedef std::chrono::steady_clock clock;
    typedef clock::time_point time_point;
    typedef clock::duration duration;

public:
    timer();
    duration split() const;
private:
    time_point start;
};

} // namespace sky

#endif // TIMER_H
