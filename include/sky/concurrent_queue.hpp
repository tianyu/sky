#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <queue>
#include <utility>
#include <type_traits>

namespace sky {

/** @brief A thread-safe queue
 *
 * Currently, this class is simply a thread-safe wrapper around std::queue;
 * However, the following operations are disabled:
 *  - copying
 *  - back()
 *  - size()
 */
template<typename T>
class concurrent_queue
{
public:
    typedef std::queue<T> container_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;

    concurrent_queue();

    template<typename Alloc>
    explicit concurrent_queue(Alloc &&alloc);

    concurrent_queue(concurrent_queue const&) = delete;

    concurrent_queue(concurrent_queue &&);

    concurrent_queue &operator =(concurrent_queue const&) = delete;

    concurrent_queue &operator =(concurrent_queue &&);

    value_type pop();

    void push(T const& value);
    void push(T&& value);

    template<typename... Args>
    void emplace(Args... args);

    bool empty() const;

    void swap(concurrent_queue const& other);

private:
    container_type queue;
};

template<typename T>
concurrent_queue<T>::
concurrent_queue() {}

template<typename T>
template<typename Alloc>
concurrent_queue<T>::
concurrent_queue(Alloc &&alloc) :
    queue(std::forward<Alloc>(alloc))
{}

template<typename T>
concurrent_queue<T>::
concurrent_queue(concurrent_queue &&other) :
    queue(std::move(other.queue))
{}

template<typename T>
concurrent_queue<T> &
concurrent_queue<T>::
operator =(concurrent_queue &&other)
{
    queue = std::move(other.queue);
    return *this;
}

template<typename T>
typename concurrent_queue<T>::value_type
concurrent_queue<T>::
pop()
{
    value_type value = queue.front();
    queue.pop(); // Exception safe?
    return value;
}

template<typename T>
void
concurrent_queue<T>::
push(T const& value)
{
    queue.push(value);
}

template<typename T>
void
concurrent_queue<T>::
push(T && value)
{
    queue.push(std::move(value));
}

template<typename T>
bool
concurrent_queue<T>::
empty() const
{
    return queue.empty();
}

template<typename T>
void
concurrent_queue<T>::
swap(concurrent_queue const& other)
{
    using std::swap;
    swap(queue, other.queue);
}

} // namespace sky

#endif // CONCURRENT_QUEUE_HPP
