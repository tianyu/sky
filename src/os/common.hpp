#ifndef COMMON_HPP
#define COMMON_HPP

#include <unistd.h>
#include <cerrno>
#include <stdexcept>
#include <system_error>

namespace {

std::system_error make_system_error(int error, char const* msg = nullptr)
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

size_t write_fd(int fd, void const* buf, size_t count,
                unsigned try_again = MAX_TRY_TIMES)
{
    int nwrote = ::write(fd, buf, count);
    if (nwrote != -1) return nwrote;

    switch (errno) {
    case EBADF:
        throw std::invalid_argument("write_fd: Bad file descriptor");
    case EINVAL:
        throw std::invalid_argument("write_fd: "
            "Either the file is unsuitable for writing or "
            "the file was opened with the O_DIRECT flag and either "
                "the address specified in buf, "
                "the value specified in count, "
                "or the current file offset "
                "is not suitably aligned.");
    case EDESTADDRREQ:
        throw std::invalid_argument("write_fd: "
            "The file is a datagram socket for which a peer address has not "
            "been set using connect().");
    case EFAULT:
        throw std::out_of_range("write_fd: "
            "The buffer to write is outside the accessible address space.");
    case EFBIG:
        throw std::length_error("write_fd: "
            "Either the file or the offset of the write position is too big.");
    case EAGAIN:
#if EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
        throw make_system_error(errno, "write_fd: "
            "Write would have caused non-blocking file to block.");
    case EPIPE:
        throw make_system_error(EPIPE, "write_fd: Broken pipe.");
    case ENOSPC:
        throw make_system_error(ENOSPC,
            "write_fd: No space in the underlying device.");
    case EIO:
        throw make_system_error(EIO, "write_fd: I/O error.");
    case EINTR:
        if (!try_again)
            throw make_system_error(EINTR, "write_fd: Interrupted.");
        return write_fd(fd, buf, count, try_again - 1);
    default:
        throw make_system_error(errno, "write_fd: Unknown error.");
    }
}

size_t read_fd(int fd, void *buf, size_t count,
               unsigned try_again = MAX_TRY_TIMES)
{
    int nread = ::read(fd, buf, count);
    if (nread != -1) return nread;

    switch (errno) {
    case EBADF:
        throw std::invalid_argument("read_fd: Bad file descriptor.");
    case EISDIR:
        throw std::invalid_argument("read_fd: File is a directory.");
    case EINVAL:
        throw std::invalid_argument("read_fd: "
            "Either the file is unsuitable for writing or "
            "the file was opened with the O_DIRECT flag and either "
                "the address specified in buf, "
                "the value specified in count, "
                "or the current file offset "
                "is not suitably aligned.");
    case EFAULT:
        throw std::out_of_range("read_fd: "
            "The buffer to write is outside the accessible address space.");
    case EAGAIN:
#if EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
        throw make_system_error(errno, "read_fd: "
            "Write would have caused non-blocking file to block.");
    case EIO:
        throw make_system_error(EIO, "read_fd: I/O error.");
    case EINTR:
        if (!try_again)
            throw make_system_error(EINTR, "read_fd: Interrupted.");
        return read_fd(fd, buf, count, try_again - 1);
    default:
        throw make_system_error(errno, "read_fd: Unknown error.");
    }
}

void dup_fd(int newfd, int oldfd,
            unsigned try_again = MAX_TRY_TIMES)
{
    if (::dup2(oldfd, newfd) != -1) return;

    switch (errno) {
    case EBADF:
        throw std::invalid_argument("dup_fd: Bad file descriptor.");
    case EBUSY:
        throw make_system_error(EBUSY, "dup_fd: Race condition detected.");
    case EINTR:
        if (!try_again)
            throw make_system_error(EINTR, "dup_fd: Interrupted.");
        dup_fd(newfd, oldfd, try_again - 1);
        return;
    case EMFILE:
        throw make_system_error(EMFILE,
            "dup_fd: Too many file descriptors in the process.");
    default:
        throw make_system_error(errno, "dup_fd: Unknown error.");
    }
}

} // namespace

#endif // COMMON_HPP
