#include "common.hpp"

#include "sky/os.h"

namespace sky {

void _::execvp(input in, output out, output err,
               char const*name, char const*const args[])
{
    stdin.dup(in);
    stdout.dup(out);
    stderr.dup(err);

    if (!in.is_standard()) in.close();
    if (!out.is_standard()) out.close();
    if (!err.is_standard()) err.close();

    ::execvp(name, const_cast<char *const*>(args));

    // execvp should not return. If it does, it sets errno.
    switch (errno) {
    case E2BIG:
    case EACCES:
    case EFAULT:
    case EINVAL:
    case EIO:
    case EISDIR:
    case ELIBBAD:
    case ELOOP:
    case EMFILE:
    case ENAMETOOLONG:
    case ENFILE:
    case ENOEXEC:
    case ENOMEM:
    case ENOTDIR:
    case EPERM:
    case ETXTBSY:
        throw errno;
    }
}

void _::forkvp(input in, output out, output err,
               const char *name, const char * const args[])
{
    input forkin = in.is_standard()? in.dup() : in;
    output forkout = out.is_standard()? out.dup() : out;
    output forkerr = err.is_standard()? err.dup() : err;

    pid_t pid = ::fork();
    if (pid == 0) {
        // Child process
        execvp(forkin, forkout, forkerr, name, args);
    } else if (pid != -1) {
        // Parent process
        if (in.is_standard()) forkin.close();
        if (out.is_standard()) forkout.close();
        if (err.is_standard()) forkerr.close();
        return;
    }

    switch (errno) {
    case EAGAIN:
    case ENOMEM:
    case ENOSYS:
        throw errno;
    }
}

} // namespace sky
