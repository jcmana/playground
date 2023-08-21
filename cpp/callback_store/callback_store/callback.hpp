#pragma once

#include <utility>

#include "../../link/link/link_element.hpp"

/// \brief      Scope-guarded `Callable` wrapper over `Callable` type.
/// \param      T       Functor storage type.
template<typename T>
class callback : private link_element
{
public:
    /// \brief      Default constructor, creates inactive callback.
    callback() noexcept;

    /// \brief      Constructor, creates active callback with `callable`.
    callback(T callable, link_element element) noexcept;

    /// \brief      `true` for invokable callback, `false` otherwise.
    bool active() const;

    /// \brief      Invokes callback's `callable` if `active()` with `args`.
    template<typename ... A>
    void invoke(A ... args);

private:
    T m_callable;
};

#pragma region callback_implementation:

template<typename T>
callback<T>::callback() noexcept :
    m_callable()
{
}

template<typename T>
callback<T>::callback(T callable, link_element element) noexcept :
    link_element(std::move(element)),
    m_callable(std::move(callable))
{
}

template<typename T>
bool 
callback<T>::active() const
{
    return link_element::linked();
}

template<typename T>
template<typename ... A>
void 
callback<T>::invoke(A ... args)
{
    if (active())
    {
        m_callable(std::forward<A>(args) ...);
    }
}

#pragma endregion
