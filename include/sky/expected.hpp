#ifndef EXPECTED_HPP
#define EXPECTED_HPP

#include <exception>
#include <utility>

namespace sky {

class error
{
    template<typename T>
    friend class expected;

public:
    error() :
        ex(std::current_exception())
    {}

    template<typename E>
    error(E &&e) :
        ex(std::make_exception_ptr(
                std::forward<E>(e)))
    {}

private:
    std::exception_ptr ex;
};

template<typename T>
class expected
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

    expected(error &&err) :
        err(std::move(err.ex)),
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
