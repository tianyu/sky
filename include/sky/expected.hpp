#ifndef EXPECTED_HPP
#define EXPECTED_HPP

#include <exception>
#include <utility>

namespace sky {

class unexpected
{
    template<typename T>
    friend class expected;

public:
    unexpected() :
        error(std::current_exception())
    {}

    template<typename E>
    unexpected(E &&e) :
        error(std::make_exception_ptr(
                std::forward<E>(e)))
    {}

private:
    std::exception_ptr error;
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

    expected(unexpected &&e) :
        error(std::move(e.error)),
        _valid(false)
    {}

    expected(expected const& e) :
        _valid(e._valid)
    {
        if (_valid) {
            new (&value) ValueType(e.value);
        } else {
            new (&error) std::exception_ptr(e.error);
        }
    }

    expected(expected &&e) :
        _valid(e._valid)
    {
        if (_valid) {
            new (&value) ValueType(std::move(e.value));
        } else {
            new (&error) std::exception_ptr(std::move(e.error));
        }
    }

    bool valid() const
    {
        return _valid;
    }

    void rethrow() const
    {
        if (_valid) return;
        std::rethrow_exception(error);
    }

    operator ValueType &()
    {
        if (_valid) return value;
        std::rethrow_exception(error);
    }

    operator ValueType const&() const
    {
        if (_valid) return value;
        std::rethrow_exception(error);
    }

    ~expected()
    {
        if (_valid) {
            value.~ValueType();
        } else {
            using std::exception_ptr;
            error.~exception_ptr();
        }
    }

private:
    union {
        ValueType value;
        std::exception_ptr error;
    };
    bool _valid;
};

} // namespace sky

#endif // EXPECTED_HPP
