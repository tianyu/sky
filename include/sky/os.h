#ifndef OS_H
#define OS_H

#include <tuple>

namespace sky {

class input
{
public:
    explicit input(int fd);

    void close();

private:
    int fd;
};

class output
{
public:
    explicit output(int fd);

    void close();

private:
    int fd;
};

std::tuple<input, output> make_pipe();

} // namespace sky

#endif // OS_H
