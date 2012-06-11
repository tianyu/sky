#ifndef TIME_H
#define TIME_H

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
    long s;
    long ns;
};

} // namespace sky

#endif // TIME_H
