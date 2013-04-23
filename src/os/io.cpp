#include "common.hpp"
#include "sky/os.h"

namespace sky {

input::input(int fd) :
    fd(fd)
{}

void input::close()
{
    close_fd(fd);
}

output::output(int fd) :
    fd(fd)
{}

void output::close()
{
    close_fd(fd);
}

} // namespace sky;
