#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP

#include <utility>

namespace sky {

template<typename Routine>
class __scope_guard;

/**
 * @brief Creates a scope guard on the stack.
 *
 * A scope guard is a lightweight RAII object that
 * can perform arbitrary actions upon destruction,
 * even in the face of exceptions.
 *
 * These actions may be dismissed (not performed
 * at destruction) if the dismiss method is called
 * on the scope guard.
 *
 * @param todo Guarded actions to perform upon
 * destruction in the form of a callable object.
 *
 * @return An appropriate scope guard object,
 * allocated unto the stack.
 */
template<typename Routine>
__scope_guard<Routine>
scope_guard(Routine &&todo);

template<typename Routine>
class __scope_guard
{
    // Copy and move constructors. Private use only.

    template<typename R>
    explicit __scope_guard(R &&);

    friend
    __scope_guard<Routine>
    scope_guard<Routine>(Routine &&);

public:

    // Scope guards are moveable but not copyable

    __scope_guard(__scope_guard &&g);
    __scope_guard &operator =(__scope_guard &&g) noexcept;

    __scope_guard(__scope_guard const&) = delete;
    __scope_guard &operator =(__scope_guard const&) = delete;

    /**
     * Causes the destructor to no longer run guarded routine.
     */
    void dismiss() noexcept;

    ~__scope_guard();

private:
    Routine todo;
    bool perform = true;
};

template<typename Routine>
__scope_guard<Routine>
scope_guard(Routine &&todo)
{
    return __scope_guard<Routine>(
                std::forward<Routine>(todo));
}

template<typename Routine>
template<typename R>
__scope_guard<Routine>::
__scope_guard(R &&todo) :
todo(std::forward<R>(todo))
{}

template<typename Routine>
__scope_guard<Routine>::
__scope_guard(__scope_guard &&g) :
todo(g.todo),
perform(g.perform)
{
    g.dismiss();
}

template<typename Routine>
__scope_guard<Routine> &
__scope_guard<Routine>::
operator =(__scope_guard &&g) noexcept
{
    using std::swap;
    swap(todo, g.todo);
    perform = g.perform;
    g.dismiss();
    return *this;
}

template<typename Routine>
void
__scope_guard<Routine>::
dismiss() noexcept
{
    perform = false;
}

template<typename Routine>
__scope_guard<Routine>::
~__scope_guard()
{
    if (perform) todo();
}

} // namespace sky

#endif // SCOPE_GUARD_HPP
