#ifndef COMMON_HPP
#define COMMON_HPP

#include <unistd.h>
#include <cerrno>
#include <stdexcept>
#include <system_error>

namespace {

std::system_error make_system_error(int error, char const* msg)
{
    return std::system_error(error, std::system_category(), msg);
}

enum { MAX_TRY_TIMES = 10 };

void close_fd(int fd, unsigned try_again = MAX_TRY_TIMES)
{
    if (::close(fd) == 0) return;
    switch (errno) {
    case EBADF:
        throw std::invalid_argument("close_fd: Bad file descriptor.");
    case EINTR:
        if (!try_again)
            throw make_system_error(EINTR, "close_fd: Interrupted.");
        close_fd(fd, try_again - 1);
        return;
    case EIO:
        throw make_system_error(EIO, "close_fd: I/O error.");
    default:
        throw make_system_error(errno, "close_fd: Unknown error.");
    }
}

} // namespace

#endif // COMMON_HPP
