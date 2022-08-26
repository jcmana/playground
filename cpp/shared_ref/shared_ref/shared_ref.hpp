#pragma once

#include <stdexcept>
#include <memory>
#include <utility>

/// \brief      `shared_ptr` that cannot be `nullptr` or reseated.
template<typename T>
class shared_ref
{
public:
    /// \brief      Default constructor, deleted.
    shared_ref() = delete;

    /// \brief      Constructor, new `T` using forwarding constructor.
    template<typename ... A>
    explicit shared_ref(A ... args) :
        m_sp(new T(std::forward<A>(args) ...))
    {
    }

    /// \brief      Copy constructor.
    shared_ref(const shared_ref & other) noexcept = default;

    /// \brief      Move constructor.
    shared_ref(shared_ref && other) noexcept :
        m_sp(other.m_sp)
    {
    }

    /// \brief      Constructor, copies reference from `sp`.
    /// \throws     `std::invalid_argument`     `sp` was `nullptr`.
    explicit shared_ref(std::shared_ptr<T> sp) :
        m_sp(std::move(sp))
    {
        if (m_sp == nullptr)
        {
            throw std::invalid_argument("shared_ptr was nullptr");
        }
    }

    /// \brief      Copy assignment, deleted.
    shared_ref & operator  =(const shared_ref & other) noexcept = delete;
    /// \brief      Move assignment, deleted.
    shared_ref & operator  =(shared_ref && other) noexcept = delete;

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

    /// \returns    Stored reference.
    T & get() const noexcept
    {
        return (*m_sp);
    }

private:
    const std::shared_ptr<T> m_sp;
};
