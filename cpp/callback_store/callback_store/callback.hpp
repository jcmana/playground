#pragma once

#include <functional>
#include <utility>
#include <type_traits>

#include "../../link/link/link_element.hpp"

/// \brief      Scope-guarded callback SFINAE template.
template <typename T, typename E = void>
class callback;

/// \brief      Scope-guarded callback to interface.
///
/// Callback is bound to a interface pointer and allows invoking methods
/// from it. Callback requires `callback_guard` to be alive, otherwise it won't
/// be invoked.
template<typename T>
class callback<T, typename std::enable_if_t<std::is_invocable_v<T> == false>> :
    private link_element
{
public:
    /// \brief      Default constructor, creates inactive callback.
    callback() :
        m_interface_ptr(nullptr)
    {
    }

    /// \brief      Constructor, creates active callback.
    callback(T & inteface_ref, link_element && link_element_rref) :
        link_element(std::move(link_element_rref)),
        m_interface_ptr(&inteface_ref)
    {
    }

    /// \brief      Invokes method from `T`, if the callback is still active.
    template<typename F, typename ... A>
    void
    invoke(F method_ptr, A && ... args) const
    {
        if (link_element::linked())
        {
            (m_interface_ptr->*method_ptr)(std::forward<A>(args) ...);
        }
    }

private:
    T * m_interface_ptr;
};

/// \brief      Scope-guarded callback to invocable.
template<typename F>
class callback<F, typename std::enable_if_t<std::is_invocable_v<F> == true>> :
    private link_element
{
public:
    /// \brief      Default constructor, creates inactive callback.
    callback()
    {
    }

    /// \brief      Constructor, creates active callback.
    callback(F functor, link_element && link_element_rref) :
        link_element(std::move(link_element_rref)),
        m_functor(std::move(functor))
    {
    }
    /// \brief      Invokes function `T`, if the callback is still active.
    template<typename ... A>
    void invoke(A && ... args) const
    {
        m_functor(std::forward<A>(args) ...);
    }

private:
    F m_functor;
};
