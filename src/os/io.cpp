#include "common.hpp"
#include "sky/os.h"

namespace sky {

size_t output::write(void const*buf, size_t count) const
{
    return write_fd(fd, buf, count);
}

void output::dup(output in) const
{
    dup_fd(fd, in.fd);
}

output output::dup() const
{
    return output(dup_fd(fd));
}

bool output::is_standard() const
{
    return is_standard_fd(fd);
}

void output::close() const
{
    close_fd(fd);
}

size_t input::read(void *buf, size_t count) const
{
    return read_fd(fd, buf, count);
}

void input::dup(input out) const
{
    dup_fd(fd, out.fd);
}

input input::dup() const
{
    return input(dup_fd(fd));
}

bool input::is_standard() const
{
    return is_standard_fd(fd);
}

void input::close() const
{
    close_fd(fd);
}

constexpr const input stdin {STDIN_FILENO};
constexpr const output stdout {STDOUT_FILENO};
constexpr const output stderr {STDERR_FILENO};

} // namespace sky;
