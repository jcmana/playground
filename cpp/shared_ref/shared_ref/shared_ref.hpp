#pragma once

#include <stdexcept>
#include <memory>
#include <utility>

/// \brief      `shared_ptr` that cannot be `nullptr` or reseated.
template<typename T>
class shared_ref
{
public:
    /// \brief      Constructor, new `T` using forwarding constructor.
    template<typename ... A>
    shared_ref(A ... args) :
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

    shared_ref & operator  =(const shared_ref & other) noexcept = default;

    /// \brief      Move constructor.
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

    T * get() const noexcept
    {
        return m_sp.get();
    }

    template<typename TF>
    friend shared_ref<TF> from_shared_ptr(std::shared_ptr<TF> sp);

private:
    shared_ref(std::shared_ptr<T> sp) noexcept :
        m_sp(std::move(sp))
    {
    }

private:
    std::shared_ptr<T> m_sp;
};

/// \brief          Constructs `shared_ref` from `shared_ptr`.
/// \throws         std::invalid_argument       If `shared_ptr` is `nullptr`.
/// 
/// `shared_ref` will refer to the same object as passed `shared_ptr` and will
/// prolong its lifetime even after the original `shared_ptr` is destroyed.
template<typename T>
shared_ref<T> from_shared_ptr(std::shared_ptr<T> sp)
{
    if (sp == nullptr)
    {
        throw std::invalid_argument("shared_ptr was nullptr");
    }

    return shared_ref<T>(std::move(sp));
}