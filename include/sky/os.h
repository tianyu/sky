#ifndef OS_H
#define OS_H

#include <tuple>

namespace sky {

class input
{
public:
    input();
    input(input const&) = default;
    explicit input(int fd);

    size_t write(void const*buf, size_t count) const;

    template<typename T, size_t N>
    size_t write(T const(&buf)[N]) const
    {
        return write(&buf, N*sizeof(T));
    }

    void close();

private:
    int fd;
};

class output
{
public:
    output();
    output(output const&) = default;
    explicit output(int fd);

    size_t read(void *buf, size_t count) const;

    template<typename T, size_t N>
    size_t read(T (&buf)[N]) const
    {
        return read(&buf, N*sizeof(T));
    }

    void close();

private:
    int fd;
};

std::tuple<input, output> make_pipe();

} // namespace sky

#endif // OS_H
