#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <queue>

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

    reference front();
    const_reference front() const;

    void pop();

    bool empty() const;

    void push(const T& value);
    void push(T&& value);

    template<typename... Args>
    void emplace(Args... args);

    void swap(const concurrent_queue& other);

private:
    container_type queue;
};

} // namespace sky

#endif // CONCURRENT_QUEUE_HPP
