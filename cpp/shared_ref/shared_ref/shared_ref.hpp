#pragma once

#include <memory>
#include <stdexcept>

/// \brief      `shared_ptr` that cannot be `nullptr` or reseated.
template<typename T>
class shared_ref
{
public:
    shared_ref(std::shared_ptr<T> sp) :
        m_sp(sp)
    {
        if (m_sp == nullptr)
        {
            throw std::invalid_argument("shared_ptr was nullptr");
        }
    }

    shared_ref(const shared_ref & other) noexcept = default;
    shared_ref(shared_ref && other) noexcept = default;

    shared_ref & operator  =(const shared_ref & other) = delete;
    shared_ref & operator  =(shared_ref && other) = delete;

    operator std::shared_ptr<T>() const noexcept
    {
        return m_sp;
    }

    T & operator  *() noexcept
    {
        return (*m_sp);
    }

    T * operator ->() noexcept
    {
        return m_sp.get();
    }

    const T & operator  *() const noexcept
    {
        return (*m_sp);
    }

    const T * operator ->() const noexcept
    {
        return m_sp.get();
    }

private:
    std::shared_ptr<T> m_sp;
};
