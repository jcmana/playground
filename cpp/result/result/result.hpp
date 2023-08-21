#pragma once

#include <variant>
#include <utility>

template<typename T, typename E>
class result
{
public:
    result() :
        m_result(std::monostate())
    {
    }

    result(T value) :
        m_result(std::move(value))
    {
    }
    result(E error) :
        m_result(std::move(error))
    {
    }

    operator bool() const
    {
        return std::holds_alternative<T>(m_result);
    }

    const T & value() const
    {
        return std::get<T>(m_result);
    }

private:
    std::variant<T, E, std::monostate> m_result;
};