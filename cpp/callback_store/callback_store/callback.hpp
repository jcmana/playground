#pragma once

#include <utility>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback : private link_element
{
public:
    /// \brief      Default constructor, creates inactive callback.
    callback()
    {
    }

    callback(T functor, link_element element) :
        link_element(std::move(element)),
        m_functor(std::move(functor))
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
            m_functor(std::forward<A>(args) ...);
        }
    }

private:
    T m_functor;
};
