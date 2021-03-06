#pragma once

#include <memory>

/// \brief      `unique_ptr` that cannot be `nullptr` or reseated.
template<typename T>
class unique_ref
{
public:
    /// \brief      Constructor, new `T` using forwarding constructor.
    template<typename ... A>
    unique_ref(A ... args) noexcept :
        m_up(new T(std::forward<A>(args) ...))
    {
    }

    unique_ref(const unique_ref & other) noexcept = delete;

    // !!!
    // MOVE SEMANTICS BREAK THE CLASS AS IT REQUIRES INTRODUCTION OF NULL STATE

    unique_ref(unique_ref && other) noexcept = default;

    unique_ref & operator  =(const unique_ref & other) = delete;
    unique_ref & operator  =(unique_ref && other) = delete;

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
