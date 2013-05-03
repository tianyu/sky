#ifndef EXPECTED_HPP
#define EXPECTED_HPP

#include <exception>
#include <utility>

namespace sky {

namespace _{

template<typename E>
class error
{
    template<typename, typename>
    friend class expected;

public:
    error(E const&ex) : ex(ex) {}
    error(E &&ex) : ex(std::move(ex)) {}

private:
    E ex;
};

template<> class error<void> {};

} // namespace _

template<typename E>
_::error<typename std::remove_reference<E>::type>
error(E ex)
{
    return _::error<typename std::remove_reference<E>::type>(std::forward<E>(ex));
}

_::error<void> error()
{
    return _::error<void>();
}

template<typename T, typename E = void>
class expected
{
public:
    typedef typename std::remove_reference<T>::type ValueType;
    typedef typename std::remove_reference<E>::type ErrorType;

    expected(ValueType const& val) :
        value(val),
        _valid(true)
    {}

    expected(ValueType &&val) :
        value(std::move(val)),
        _valid(true)
    {}

    expected(_::error<ErrorType> &&err) :
        err(std::move(err.ex)),
        _valid(false)
    {}

    expected(expected const& e) :
        _valid(e._valid)
    {
        if (_valid) {
            new (&value) ValueType(e.value);
        } else {
            new (&err) ErrorType(e.err);
        }
    }

    expected(expected &&e) :
        _valid(e._valid)
    {
        if (_valid) {
            new (&value) ValueType(std::move(e.value));
        } else {
            new (&err) ErrorType(std::move(e.err));
        }
    }

    bool valid() const
    {
        return _valid;
    }

    void rethrow() const
    {
        if (_valid) return;
        throw err;
    }

    operator ValueType &()
    {
        rethrow();
        return value;
    }

    operator ValueType const&() const
    {
        rethrow();
        return value;
    }

    ~expected()
    {
        if (_valid) {
            value.~ValueType();
        } else {
            err.~ErrorType();
        }
    }

private:
    union {
        ValueType value;
        ErrorType err;
    };
    bool _valid;
};

template<typename T>
class expected<T, void>
{
public:
    typedef typename std::remove_reference<T>::type ValueType;

    expected(ValueType const& val) :
        value(val),
        _valid(true)
    {}

    expected(ValueType &&val) :
        value(std::move(val)),
        _valid(true)
    {}

    template<typename E>
    expected(_::error<E> &&err) :
        err(std::make_exception_ptr(std::move(err.ex))),
        _valid(false)
    {}

    expected(_::error<void> &&) :
        err(std::current_exception()),
        _valid(false)
    {}

    expected(expected const& e) :
        _valid(e._valid)
    {
        if (_valid) {
            new (&value) ValueType(e.value);
        } else {
            new (&err) std::exception_ptr(e.err);
        }
    }

    expected(expected &&e) :
        _valid(e._valid)
    {
        if (_valid) {
            new (&value) ValueType(std::move(e.value));
        } else {
            new (&err) std::exception_ptr(std::move(e.err));
        }
    }

    bool valid() const
    {
        return _valid;
    }

    void rethrow() const
    {
        if (_valid) return;
        std::rethrow_exception(err);
    }

    operator ValueType &()
    {
        if (_valid) return value;
        std::rethrow_exception(err);
    }

    operator ValueType const&() const
    {
        if (_valid) return value;
        std::rethrow_exception(err);
    }

    ~expected()
    {
        if (_valid) {
            value.~ValueType();
        } else {
            using std::exception_ptr;
            err.~exception_ptr();
        }
    }

private:
    union {
        ValueType value;
        std::exception_ptr err;
    };
    bool _valid;
};

} // namespace sky

#endif // EXPECTED_HPP
