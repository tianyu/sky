#include "common.hpp"
#include "sky/os.h"

namespace sky {

input::input() :
    fd(-1)
{}

size_t input::write(void const*buf, size_t count) const
{
    return write_fd(fd, buf, count);
}

void input::dup(input in) const
{
    dup_fd(fd, in.fd);
}

void input::close()
{
    close_fd(fd);
}

output::output() :
    fd(fd)
{}

size_t output::read(void *buf, size_t count) const
{
    return read_fd(fd, buf, count);
}

void output::dup(output out) const
{
    dup_fd(fd, out.fd);
}

void output::close()
{
    close_fd(fd);
}

} // namespace sky;
