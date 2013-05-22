#include "common.hpp"
#include "sky/os.h"

namespace sky {

input::input() :
    fd(INVALID_FD)
{}

size_t input::write(void const*buf, size_t count) const
{
    return write_fd(fd, buf, count);
}

void input::dup(input in) const
{
    dup_fd(fd, in.fd);
}

input input::dup() const
{
    return input(dup_fd(fd));
}

void input::close()
{
    close_fd(fd);
}

output::output() :
    fd(INVALID_FD)
{}

size_t output::read(void *buf, size_t count) const
{
    return read_fd(fd, buf, count);
}

void output::dup(output out) const
{
    dup_fd(fd, out.fd);
}

output output::dup() const
{
    return output(dup_fd(fd));
}

void output::close()
{
    close_fd(fd);
}

constexpr const output stdin {STDIN_FILENO};
constexpr const input stdout {STDOUT_FILENO};
constexpr const input stderr {STDERR_FILENO};

} // namespace sky;
