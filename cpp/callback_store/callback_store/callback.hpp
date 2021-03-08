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
    callback() noexcept :
        m_callable()
    {
    }

    callback(T callable, link_element element) noexcept :
        link_element(std::move(element)),
        m_callable(std::move(callable))
    {
    }

    auto active() const
    {
        return link_element::linked();
    }

    template<typename ... A>
    void invoke(A ... args)
    {
        if (active())
        {
            m_callable(std::forward<A>(args) ...);
        }
    }

private:
    T m_callable;
};
