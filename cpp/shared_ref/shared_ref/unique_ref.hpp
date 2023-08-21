#pragma once

#include <cstddef>
#include <memory>
#include <exception>

/// \brief      `unique_ptr` that cannot be `nullptr`.
template<typename T>
class unique_ref
{
public:
    unique_ref() = delete;


    unique_ref(const unique_ref & other) noexcept = delete;
    unique_ref(unique_ref && other) noexcept = delete;

    explicit unique_ref(const T & value) :
        m_up(std::make_unique<T>(value))
    {
    }

    explicit unique_ref(T && value) :
        m_up(std::make_unique<T>(std::move(value)))
    {
    }

    explicit unique_ref(std::nullptr_t) noexcept = delete;

    explicit unique_ref(T * ptr) :
        m_up(ptr)
    {
        if (m_up == nullptr)
        {
            throw std::invalid_argument("unique_ptr was nullptr");
        }
    }

    explicit unique_ref(std::unique_ptr<T> up) :
        m_up(std::move(up))
    {
        if (m_up == nullptr)
        {
            throw std::invalid_argument("unique_ptr was nullptr");
        }
    }

    unique_ref & operator  =(const unique_ref & other) noexcept = delete;
    unique_ref & operator  =(unique_ref && other) noexcept = delete;

    T & operator  *() const noexcept
    {
        return (*m_up);
    }

    T * operator ->() const noexcept
    {
        return m_up.get();
    }

    T * get() const noexcept
    {
        return m_up.get();
    }

private:
    std::unique_ptr<T> m_up;
};
