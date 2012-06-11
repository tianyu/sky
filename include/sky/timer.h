#ifndef TIMER_H
#define TIMER_H

#include "sky/time.h"

namespace sky {
namespace perf {

class timer {
public:
    timer(const char* name);
    ~timer();
private:
    const char* name;
    time start;
};

} // namespace perf
} // namespace sky

#endif // TIMER_H
