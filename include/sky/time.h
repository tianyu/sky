#ifndef TIME_H
#define TIME_H

#include <ctime>

namespace sky {

class time {
public:
    time& operator -=(time t);
    time operator -(time t) const;
    long millis() const;
    long long nanos() const;

    static time now();
    static time epoch();
private:
    union {
        struct {
            long s;
            long ns;
        };
        timespec spec;
    };
};

} // namespace sky

#endif // TIME_H
