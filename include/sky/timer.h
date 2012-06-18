#ifndef TIMER_H
#define TIMER_H

#include "sky/time.h"

namespace sky {

class timer {
public:
    timer(const char* name);
    time duration() const;
    ~timer();
private:
    const char* name;
    time start;
};

} // namespace sky

#endif // TIMER_H
