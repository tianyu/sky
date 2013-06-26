#ifndef OS_H
#define OS_H

#include <cstddef>
#include <utility>

#include "sky/type_traits.hpp"
#include "sky/tuple.hpp"

namespace sky {

/**
 * @defgroup os Operating System Support
 */

/**
 * @defgroup io Input/Output
 * @ingroup os
 *
 * @warning Currently, only posix-compliant systems are supported by this
 * library.
 * In particular, *Windows is not supported*.
 *
 * The operating system provides handles to streams that can be used to access
 * files, sockets, pipes, etc.
 * These are called "file descriptors" on posix systems and "HANDLE's" on
 * Windows.
 * Each operating system provides a set of system calls that can be used to
 * manipulate and interact with these handles.
 *
 * The goal of this module is to provide a system-agnostic interface to
 * system-specific handles.
 * In particular:
 *
 * - @ref input objects provide an interface to handles that have
 *   *write access*.
 * - @ref output object provide an interface to handles that have
 *   *read access*.
 *
 * Collectively, we will refer to both input and output objects as I/O objects.
 *
 * ## Object States
 * Once an I/O object has been constructed, its handle will never change.
 * There are two states that an I/O object can be in, depending on the state
 * of its handle:
 *
 * 1. Valid: The handle is referring to a valid stream.
 *    Any member functions on a valid I/O object can be called.
 * 2. Invalid: The handle is not referring to a valid stream.
 *    This may happen if the I/O object was closed or if it never referred to a
 *    valid stream in the first place.
 *    Any attempt to call member functions on an invalid I/O object will cause
 *    the function to throw `std::invalid_argument`.
 *
 * In general, I/O objects returned from the @ref os library will always be
 * valid.
 * Since the state of an I/O object cannot be queried, it is up to the user to
 * stop using I/O objects after they become invalid.
 *
 * ## Const-ness and Thread Safety
 * In general, member functions of I/O objects are const.
 * This means that the handle of the I/O object is bit-wise constant, however,
 * it does not imply that each member function is thread-safe.
 *
 * ## Duplicating I/O Objects
 * An I/O object may be duplicated by calling one of its `dup` member functions.
 * For example:
 *
 *     input newinput = oldinput.dup(); // Case 1
 *     newinput.dup(oldinput); // Case 2
 *
 * However, **duplicating is not the same as copying**.
 * In general, a duplicated I/O object contains a different handle to the same
 * stream.
 * So while it is possible to read/write from either the original or the
 * duplicated I/O object, both objects should be closed when they are no longer
 * needed.
 */

/**
 * @brief The output end of a file or stream to be written to.
 * @ingroup io
 */
class output
{
public:

    output() = delete;

    output(output const&) = default;
    output(output &&) = default;

    output &operator =(output const&) = delete;
    output &operator =(output &&) = delete;

    /**
     * @brief Creates an output from a file descriptor.
     *
     * @warning This is an internal function and
     * should not be used in production code.
     *
     * @param fd A valid file descriptor.
     */
    explicit constexpr output(int fd) :
        fd(fd)
    {}

    /**
     * @brief Writes the bytes in a given buffer to the output.
     * @param buf A pointer to the start of the buffer.
     * @param count The size of the buffer in number of bytes.
     * @return The number of bytes written to the output.
     */
    size_t write(void const*buf, size_t count) const;

    /**
     * @brief Writes the bytes in a given array to the output.
     * @param buf A reference to an array to write to the buffer.
     * @return The number of bytes written to the output.
     */
    template<typename T, size_t N>
    size_t write(T const(&buf)[N]) const
    {
        return write(&buf, N*sizeof(T));
    }

    /**
     * @brief Writes the bytes of a given object to the output.
     * @param value The value to write.
     * @return The number of bytes written to the output.
     */
    template<typename T>
    size_t write(T const&value) const
    {
        return write(&value, sizeof(T));
    }

    /**
     * @brief Duplicates this output to refer to
     *        the same stream as the given output.
     *
     * After this operation completes successfully, this output and the given
     * output will refer to the same stream and may be used interchangeably.
     * Both outputs can and should be closed when they are no longer needed.
     *
     * @param in The output to duplicate.
     */
    void dup(output in) const;

    /**
     * @brief Duplicates this output.
     *
     * A new output is created that refers to the same stream as this output,
     * and both may be used interchangeably.
     * Both outputs can and should be closed when they are no longer needed.
     *
     * @return A new, duplicate output.
     */
    output dup() const;

    /**
     * @brief Determines if this output is a standard output.
     * @return true iff this output is a standard output.
     */
    bool is_standard() const;

    /**
     * @brief Close this output.
     *
     * Once the output is closed, all other calls to write() and close()
     * will throw std::invalid_argument.
     */
    void close() const;

private:
    int fd;
};

/**
 * @brief The input end of a stream to be read from.
 * @ingroup io
 */
class input
{
public:

    input() = delete;

    input(input const&) = default;
    input(input &&) = default;

    input &operator =(input const&) = delete;
    input &operator =(input &&) = delete;

    explicit constexpr input(int fd) :
        fd(fd)
    {}

    /**
     * @brief Reads bytes from the input into the given buffer.
     * @param buf A pointer to the start of the buffer.
     * @param count The size of the buffer in number of bytes.
     * @return The number of bytes read.
     */
    size_t read(void *buf, size_t count) const;

    /**
     * @brief Read bytes from the input into the given array.
     *
     * It is up to the user to ensure that the number of bytes and the type of
     * data available to be read aligns with elements of the array.
     * It is highly recommended that only POD types are used for this function.
     *
     * @param buf The array to store the read bytes.
     * @return The number of bytes read.
     */
    template<typename T, size_t N>
    size_t read(T (&buf)[N]) const
    {
        return read(&buf, N*sizeof(T));
    }

    /**
     * @brief Read bytes from the input into a given value.
     *
     * It is up to the suer to ensure that the number of bytes and the type of
     * data available to be read can be interpreted as an object of type T.
     * It is highly recommended that only POD types are used for this function.
     *
     * @param value The value to store the read bytes.
     * @return The number of bytes read.
     */
    template<typename T>
    size_t read(T &value) const
    {
        return read(&value, sizeof(T));
    }

    /**
     * @brief Duplicates this input to refer to
     *        the same stream as the given input.
     *
     * After this operation completes successfully, this input and the given
     * input will refer to the same stream and may be used interchangeably.
     * Both inputs can and should be closed when they are no longer needed.
     *
     * @param out The input to duplicate.
     */
    void dup(input out) const;

    /**
     * @brief Duplicates this input.
     *
     * A new input is created that refers to the same stream as this input,
     * and both may be used interchangeably.
     * Both inputs can and should be closed when they are no longer needed.
     *
     * @return A new, duplicate input.
     */
    input dup() const;

    /**
     * @brief Determines if this input is a standard input.
     * @return true iff this input is a standard input.
     */
    bool is_standard() const;

    /**
     * @brief Close this input.
     *
     * Once the input is closed, all other calls to read() and close() will
     * throw std::invalid_argument.
     */
    void close() const;

private:
    int fd;
};

/**
 * @brief Standard input.
 *
 * The process' standard input stream.
 * @ingroup io
 */
extern const input stdin;

/**
 * @brief Standard output.
 *
 * The process' standard output stream.
 * @ingroup io
 */
extern const output stdout;

/**
 * @brief Standard error.
 *
 * The process' standard error stream.
 * @ingroup io
 */
extern const output stderr;

/**
 * @brief Make a pipe.
 *
 * #### Example
 *
 *     #include "sky/tuple.hpp"
 *     :::
 *     auto pipe = make_pipe();
 *     auto &in = sky::get<input>(pipe);
 *     auto &out = sky::get<output>(pipe);
 *
 * @ingroup io
 * @return A tuple containing the input and output ends of the pipe.
 */
std::tuple<input, output> make_pipe();

namespace _ {

template<typename T>
typename std::enable_if<
std::is_void<decltype(
        std::declval<T>().execute(
            std::declval<input>(),
            std::declval<output>(),
            std::declval<output>())
        )>::value,
std::true_type>::type
has_execute(T);

template<typename T>
typename std::enable_if<
std::is_void<decltype(
        std::declval<T>().fork(
            std::declval<input>(),
            std::declval<output>(),
            std::declval<output>())
        )>::value,
std::true_type>::type
has_fork(T);

std::false_type has_fork(...);

struct predicate_executable
{
    template<typename T>
    using op = sky::is_same<std::true_type,
                sky::forall<
                    decltype(has_execute(std::declval<T>())),
                    decltype(has_fork   (std::declval<T>()))>>;
};

} // namespace _

/**
 * @brief Type trait to determine if a class is an executable.
 *
 * A type `T` is executable if the following code is valid:
 *
 *     t.execute(stdin, stdout, stderr);
 *     t.fork(stdin, stdout, stderr);
 *
 * For some object, `t`, of type `T`.
 *
 * If `execute` completes successfully, it will not return.
 * Instead the current process will be replaced by the process defined in the
 * executable.
 *
 * @ingroup os
 */
template<typename T>
using is_executable = sky::relate<_::predicate_executable, T>;

namespace _ {

void execvp(input in, output out, output err,
            char const*name, const char *const args[]);

void forkvp(input in, output out, output err,
            char const*name, const char *const args[]);

template<size_t N>
class cmd
{
public:
    template<typename... Args>
    constexpr cmd(char const*name, Args&&... args) :
        args{name, std::forward<Args>(args)..., nullptr}
    {}

    void execute(input in = stdin,
                 output out = stdout,
                 output err = stderr) const
    {
        execvp(in, out, err, args[0], args);
    }

    void fork(input in = stdin,
              output out = stdout,
              output err = stderr) const
    {
        forkvp(in, out, err, args[0], args);
    }

private:
    char const* args[N + 2];
};

} // namespace _


/**
 * @brief Creates an executable command.
 *
 * If @a name given is not a path, then the file to execute is found as if the
 * command was typed into a console.
 *
 * @warning All of the arguments given must decay to `char const*` or
 * compilation will fail.
 *
 * #### Examples
 *
 *     cmd("ls", "-l");
 *
 * @ingroup os
 * @param name The name of the command.
 * @param args The arguments to be passed into the command.
 *             The type of each argument must decay to `char const*`.
 * @return An executable object.
 */
template<typename... Args>
constexpr _::cmd<sizeof...(Args)>
cmd(char const* name, Args&&... args)
{
    static_assert(sky::is_same<char const*, sky::forall<
                  typename std::decay<Args>::type...>>::value,
                  "Arguments must decay to type char const*.");
    return _::cmd<sizeof...(Args)>(name, std::forward<Args>(args)...);
}

namespace _ {

template<>
constexpr _::cmd<0>
cmd<>(char const* name)
{
    return _::cmd<0>(name);
}

template<typename Src, typename Dest>
class pipe_exec
{
public:

    template<typename T, typename U>
    constexpr pipe_exec(T&& src, U&& dest) :
        src(std::forward<T>(src)),
        dest(std::forward<U>(dest))
    {}

    void execute(input in = stdin,
                 output out = stdout,
                 output err = stderr)
    {
        auto pipe = make_pipe();
        auto &pin = sky::get<input>(pipe);
        auto &pout = sky::get<output>(pipe);

        src.fork(in, pout, err);
        pout.close();
        dest.execute(pin, out, err);
    }

    void fork(input in = stdin,
                 output out = stdout,
                 output err = stderr)
    {
        auto pipe = make_pipe();
        auto &pin = sky::get<input>(pipe);
        auto &pout = sky::get<output>(pipe);

        src.fork(in, pout, err);
        pout.close();
        dest.fork(pin, out, err);
        pin.close();
    }

private:
    Src src;
    Dest dest;
};

} // namespace _

/**
 * Pipe the stdout of one executable command into the stdin of another
 * exectable command.
 *
 * #### Examples
 *
 *     auto foo2bar = foo | bar;
 *     foo2bar.execute(in, out, err);
 *
 * `foo2bar` is actually another executable.
 * When `foo2bar` is executed in the second line:
 *
 * - `foo` will read its standard input from `in`.
 * - `bar` will write its standard output to `out`.
 * - Both `foo` and `bar` will write their standard error to `err`.
 *
 * @ingroup os
 * @param src The source executable.
 * @param dest The exectuable whose standard input will receive the data written
 *             in the standard output of the source executable.
 * @return A new executable representing the source piped into the destination.
 */
template<typename Src, typename Dest>
constexpr
typename std::enable_if<
    is_executable<forall<Src, Dest>>::value,
    _::pipe_exec<Src, Dest>
>::type
operator |(Src&& src, Dest&& dest)
{
    return pipe_exec<Src, Dest>(
                std::forward<Src>(src),
                std::forward<Dest>(dest)
            );
}

} // namespace sky

#endif // OS_H
