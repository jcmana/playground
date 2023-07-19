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
    template<typename FT>
    friend class shared_ref;

public:
    /// \brief      Default constructor, deleted.
    shared_ref() = delete;

    /// \brief      Copy constructor.
    shared_ref(const shared_ref & other) noexcept = default;

    /// \brief      Move constructor.
    shared_ref(shared_ref && other) noexcept :
        m_sp(other.m_sp)
    {
    }

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

    /// \brief      Constructor, casts `sp` to `T`.
    template<typename Y>
    shared_ref(shared_ref<Y> && sr) :
        m_sp(std::static_pointer_cast<T>(sr.m_sp))
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

    /// \brief      Constructor, casts `sp` to `T`.
    /// \throws     `std::invalid_argument`     `sp` was `nullptr`.
    template<typename Y>
    explicit shared_ref(std::shared_ptr<Y> sp) :
        m_sp(std::static_pointer_cast<T>(sp))
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

    /// \brief      Conversion to `shared_ref` to `Y`.
    template<typename Y>
    operator std::shared_ptr<Y>() const noexcept
    {
        return shared_ref<Y>(m_sp);
    }

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

