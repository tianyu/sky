#include "common.hpp"
#include "sky/os.h"

namespace sky {

input::input() :
    fd(-1)
{}

input::input(int fd) :
    fd(fd)
{}

size_t input::write(void const*buf, size_t count) const
{
    return write_fd(fd, buf, count);
}

void input::close()
{
    close_fd(fd);
}

output::output() :
    fd(fd)
{}

output::output(int fd) :
    fd(fd)
{}

size_t output::read(void *buf, size_t count) const
{
    return read_fd(fd, buf, count);
}

void output::close()
{
    close_fd(fd);
}

} // namespace sky;
