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
        throw make_system_error(E2BIG,
            "execvp: The argument list is too large.");
    case EACCES:
        throw make_system_error(EACCES, "execvp: "
            "Unable to access command. "
            "Either the file is not a regular file, "
            "or execute permission is denied for the file, "
            "or the file system is mounted \"noexec\"");
    case EFAULT:
        throw std::logic_error("execvp: "
            "Command name pointer lies outside addressable space.");
    case EINVAL:
        throw make_system_error(EINVAL, "execvp: "
            "The ELF executable has more than one PT_INTERP segment.");
    case EIO:
        throw make_system_error(EIO, "execvp: An I/O error occurred.");
    case EISDIR:
        throw make_system_error(EISDIR, "execvp: "
            "An ELF interpreter was a directory.");
    case ELIBBAD:
        throw make_system_error(ELIBBAD, "execvp: "
            "An ELF interpreter was a directory.");
    case ELOOP:
        throw make_system_error(ELOOP, "execvp: "
            "Too many symbolic links were encounted "
            "while resolving the command.");
    case EMFILE:
        throw make_system_error(EMFILE, "execvp: "
            "The process has the maximum number of files open.");
    case ENAMETOOLONG:
        throw make_system_error(ENAMETOOLONG, "execvp: "
            "Command name is too long.");
    case ENFILE:
        throw make_system_error(ENFILE, "execvp: "
            "The system limit on the total number of "
            "open files has been reached.");
    case ENOEXEC:
        throw make_system_error(ENOEXEC, "execvp: "
            "An executable is not in a recognized format, "
            "is for the wrong architecture. "
            "or has some other format error that means it cannot be executed.");
    case ENOMEM:
        throw make_system_error(ENOMEM, "execvp: "
            "Insufficient kernel memory was available.");
    case ENOTDIR:
        throw make_system_error(ENOTDIR, "execvp: "
            "A component of the path prefix "
            "for the command is not a directory.");
    case EPERM:
        throw make_system_error(EPERM, "execvp: "
            "The use is not the super user and "
            "the user does not have permission to use the file.");
    case ETXTBSY:
        throw make_system_error(ETXTBSY, "execvp: "
            "The executable was open for writing by one or more processes.");
    default:
        throw make_system_error(errno, "execvp: Unknown error.");
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
        throw make_system_error(EAGAIN, "forkvp: Resource limit encountered.");
    case ENOMEM:
        throw make_system_error(ENOMEM, "forkvp: "
            "Insufficient memory for allocating necessary kernel structures.");
    case ENOSYS:
        throw make_system_error(ENOSYS, "forkbp: "
            "Fork is not supported on this system.");
    default:
        throw make_system_error(errno, "execvp: Unknown error.");
    }
}

} // namespace sky
