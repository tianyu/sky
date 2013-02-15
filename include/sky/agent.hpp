#ifndef AGENT_HPP
#define AGENT_HPP

#include <utility>
#include <functional>
#include <atomic>
#include <future>
#include "sky/concurrent_queue.hpp"

namespace sky {


class agent
{
public:

  template<typename Func>
  void agent::run(Func &&f) const;

private:
    class task
    {
    public:
        template <typename Func>
        explicit task(Func &&f);

        void operator ()() const;

    private:
        std::function<void()> f;
    };

    typedef concurrent_queue<task> queue_t;

    mutable queue_t queue;
    mutable atomic<bool> working = false;
};

template <typename Func>
agent::task::task(Func &&f) :
    f(std::forward<Func>(f))
{}

void
agent::task::
operator ()() const
{
    f();
    std::async(queue.pop());
}

template<typename Func>
void agent::run(Func &&f) const
{
    bool was_working = false;
    working.compare_exchange_strong(was_working, true);
    if (was_working) {
        // Enqueue the task
        queue.emplace(std::forward<Func>(f));
    } else {
        // Run the task directly through async
        std::async(task(std::forward<Func>(f)));
    }
}

} // namespace sky

#endif // AGENT_HPP
