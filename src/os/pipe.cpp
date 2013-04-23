#include "common.hpp"
#include "sky/os.h"

namespace sky {

std::tuple<input, output> make_pipe()
{
    int fds[2];
    if (::pipe(fds) == 0) {
        return std::make_tuple(input(fds[0]), output(fds[1]));
    }

    switch (errno) {
    case EMFILE:
        throw make_system_error(EMFILE,
            "make_pipe: Too many file descriptors in the process.");
    case ENFILE:
        throw make_system_error(ENFILE,
            "make_pipe: Too many file descriptors in the system.");

    case EFAULT: // fds is not valid (but it should be).
    case EINVAL: // Invalid flags (but there are none).
    default:
        throw make_system_error(errno, "make_tuple: Unknown error.");
    }
}

} // namespace sky
