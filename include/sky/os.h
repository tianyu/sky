#ifndef OS_H
#define OS_H

#include <tuple>

namespace sky {

/**
 * @defgroup os Operating System Support
 */

/**
 * @brief The input end of a file or stream.
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

    explicit input(int fd);

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
 * @brief The output end of a stream.
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

    explicit output(int fd);

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
 * @brief Make a pipe.
 * @ingroup os
 * @return A tuple containing the input and output ends of the pipe.
 */
std::tuple<input, output> make_pipe();

} // namespace sky

#endif // OS_H
