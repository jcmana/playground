#pragma once

#include <memory>
#include <stdexcept>

/// \brief      `shared_ptr` that cannot be `nullptr` or reseated, keeping `const`-ness.
template<typename T>
class shared_ref
{
public:
    template<typename ... A>
    shared_ref(A ... args) :
        m_sp(new T(args ...))
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
