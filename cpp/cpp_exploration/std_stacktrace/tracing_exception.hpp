#pragma once

#include <exception>
#include <stacktrace>

/// \brief      Carries `std::stracktrace` from the `throw` point.
class tracing_exception : public std::exception
{
public:
    tracing_exception(std::string_view what) :
        std::exception(what.data())
    {
        m_stacktrace = std::stacktrace::current();
    }

    tracing_exception() :
        std::exception()
    {
        m_stacktrace = std::stacktrace::current();
    }

    const std::stacktrace & trace() const
    {
        return m_stacktrace;
    }

private:
    std::stacktrace m_stacktrace;
};