#ifndef OS_H
#define OS_H

#include <tuple>
#include <utility>

#include "sky/type_traits.hpp"

namespace sky {

/**
 * @defgroup os Operating System Support
 */

/**
 * @brief The input end of a file or stream to be written to.
 * @ingroup os
 */
class input
{
public:

    /**
     * @brief Creates an invalid input object.
     *
     * Any calls to write() or close() will throw an exception.
     */
    input();

    input(input const&) = default;

    explicit constexpr input(int fd) :
        fd(fd)
    {}

    /**
     * @brief Writes the bytes in a given buffer to the input.
     * @param buf A pointer to the start of the buffer.
     * @param count The size of the buffer in number of bytes.
     * @return The number of bytes written to the input.
     */
    size_t write(void const*buf, size_t count) const;

    /**
     * @brief Writes the bytes in a given array to the input.
     * @param buf A reference to an array to write to the buffer.
     * @return The number of bytes written to the input.
     */
    template<typename T, size_t N>
    size_t write(T const(&buf)[N]) const
    {
        return write(&buf, N*sizeof(T));
    }

    /**
     * @brief Writes the bytes of a given object to the input.
     * @param value The value to write.
     * @return The number of bytes written to the input.
     */
    template<typename T>
    size_t write(T const&value) const
    {
        return write(&value, sizeof(T));
    }

    /**
     * @brief Duplicates this input to refer to
     *        the same stream as the given input.
     *
     * After this operation completes successfully, this input and the given
     * input will refer to the same stream and may be used interchangeably.
     * Both inputs can and should be closed when they are no longer needed.
     *
     * @param in The input to duplicate.
     */
    void dup(input in) const;

    /**
     * @brief Close this input.
     *
     * Once the input is closed, all other calls to write() and close()
     * will throw std::invalid_argument.
     */
    void close();

private:
    int fd;
};

/**
 * @brief The output end of a stream to be read from.
 * @ingroup os
 */
class output
{
public:
    /**
     * @brief Creates an invalid output object.
     */
    output();

    output(output const&) = default;

    explicit constexpr output(int fd) :
        fd(fd)
    {}

    /**
     * @brief Reads bytes from the output into the given buffer.
     * @param buf A pointer to the start of the buffer.
     * @param count The size of the buffer in number of bytes.
     * @return The number of bytes read.
     */
    size_t read(void *buf, size_t count) const;

    /**
     * @brief Read bytes from the output into the given array.
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
     * @brief Read bytes from the output into a given value.
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
     * @brief Duplicates this output to refer to
     *        the same stream as the given output.
     *
     * After this operation completes successfully, this output and the given
     * output will refer to the same stream and may be used interchangeably.
     * Both outputs can and should be closed when they are no longer needed.
     *
     * @param out The output to duplicate.
     */
    void dup(output out) const;

    /**
     * @brief Close this output.
     *
     * Once the output is closed, all other calls to read() and close() will
     * throw std::invalid_argument.
     */
    void close();

private:
    int fd;
};

/**
 * @brief Standard input.
 *
 * The output end of the process' standard input stream.
 * @ingroup os
 */
constexpr const output stdin  {0};

/**
 * @brief Standard output.
 *
 * The input end of this process' standard output stream.
 * @ingroup os
 */
constexpr const input stdout {1};

/**
 * @brief Standard error.
 *
 * The input end of this process' standard error stream.
 * @ingroup os
 */
constexpr const input stderr {2};

/**
 * @brief Make a pipe.
 *
 * #### Example
 *
 *     input in;
 *     output out;
 *     std::tie<in, out> = make_pipe();
 *
 * @ingroup os
 * @return A tuple containing the input and output ends of the pipe.
 */
std::tuple<input, output> make_pipe();

namespace _ {

template<typename T>
typename std::enable_if<
std::is_void<decltype(
        std::declval<T>().execute(
            std::declval<output>(),
            std::declval<input>(),
            std::declval<input>())
        )>::value,
std::true_type>::type
has_execute(T);

std::false_type has_execute(...);

template<typename HasExecute>
class is_executable_helper :
        public std::integral_constant<bool, HasExecute::value>
{};

} // namespace _

/**
 * @brief Type trait to determine if a class is an executable.
 *
 * A type `T` is executable if the following code is valid:
 *
 *     t.execute(input(), output(), output());
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
class is_executable :
        public _::is_executable_helper<
            decltype(_::has_execute(std::declval<T>()))>
{};

namespace _ {

void execvp(char const*name, const char * const args[]);

template<size_t N>
class cmd
{
public:
    template<typename... Args>
    constexpr cmd(char const*name, Args&&... args) :
        args{name, std::forward<Args>(args)..., nullptr}
    {}

    void execute(output in = stdin,
                 input out = stdout,
                 input err = stderr) const
    {
        stdin.dup(in);
        stdout.dup(out);
        stderr.dup(err);
        execvp(args[0], args);
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
    static_assert(sky::is_same<char const*,
                  typename std::decay<Args>::type...>::value,
                  "Arguments must decay to type char const*.");
    return _::cmd<sizeof...(Args)>(name, std::forward<Args>(args)...);
}

template<>
constexpr _::cmd<0>
cmd<>(char const* name)
{
    return _::cmd<0>(name);
}

} // namespace sky

#endif // OS_H
