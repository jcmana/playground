#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <exception>

/// \brief      `shared_ptr` that cannot be `nullptr`.
template<typename T>
class shared_ref
{
public:
    /// \brief      Default constructor, deleted.
    shared_ref() = delete;

    /// \brief      Copy constructor.
    shared_ref(const shared_ref & other) noexcept = default;

    /// \brief      Move constructor, deleted.
    shared_ref(shared_ref && other) noexcept = delete;

    /// \brief      Constructor, from `nullptr`, deleted.
    explicit shared_ref(std::nullptr_t) noexcept = delete;

    /// \brief      Constructor, value copy initialization.
    explicit shared_ref(const T & value) :
        m_sp(std::make_shared<T>(value))
    {
    }

    /// \brief      Constructor, value move initialization.
    explicit shared_ref(T && value) :
        m_sp(std::make_shared<T>(std::move(value)))
    {
    }

    /// \brief      Constructor, from owning `ptr`.
    /// \throws     `std::invalid_argument`     `ptr` was `nullptr`.
    explicit shared_ref(T * ptr) :
        m_sp(ptr)
    {
        if (m_sp == nullptr)
        {
            throw std::invalid_argument("shared_ptr was nullptr");
        }
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

    /// \brief      Copy assignment.
    shared_ref & operator  =(const shared_ref & other) noexcept = default;

    /// \brief      Move assignment.
    shared_ref & operator  =(shared_ref && other) noexcept = delete;

    /// \brief      Conversion to `shared_ptr`.
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
    std::shared_ptr<T> m_sp;
};

