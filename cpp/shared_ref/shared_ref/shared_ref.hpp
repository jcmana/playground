#pragma once

#include <stdexcept>
#include <memory>
#include <utility>

/// \brief      `shared_ptr` that cannot be `nullptr` or reseated.
template<typename T>
class shared_ref
{
public:
    /// \brief      Constructor, new `T` using constructor.
    template<typename ... A>
    shared_ref(A ... args) noexcept :
        m_sp(new T(args ...))
    {
    }

    /// \brief      Constructor, new `T` using copy constructor.
    shared_ref(const T & other) noexcept :
        m_sp(new T(other))
    {
    }

    /// \brief      Constructor, new `T` using move constructor.
    shared_ref(T && other) noexcept :
        m_sp(new T(std::move(other)))
    {
    }

    shared_ref(const shared_ref & other) noexcept = default;
    shared_ref(shared_ref && other) noexcept = default;

    shared_ref & operator  =(const shared_ref & other) = delete;
    shared_ref & operator  =(shared_ref && other) = delete;

    operator std::shared_ptr<T>() const noexcept
    {
        return m_sp;
    }

    T & operator  *() const noexcept
    {
        return (*m_sp);
    }

    T * operator ->() const noexcept
    {
        return m_sp.get();
    }

private:
    std::shared_ptr<T> m_sp;
};
