#include "common.hpp"

#include "sky/os.h"

namespace sky {

void _::execvp(input in, output out, output err,
               char const*name, char const*const args[])
{
    stdin.dup(in);
    stdout.dup(out);
    stderr.dup(err);

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

} // namespace sky
