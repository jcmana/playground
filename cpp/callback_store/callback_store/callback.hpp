#pragma once

#include <functional>
#include <utility>
#include <type_traits>

#include "../../link/link/link_element.hpp"

/// \brief      Scope-guarded callback SFINAE template.
template <typename T, typename E = void>
class callback;

/// \brief      Scope-guarded callback to invocable.
/// \param      F       Functor invocation signature (same as std::function).
template<typename ... A>
class callback<void(A ...)> :
    private link_element
{
public:
    /// \brief      Default constructor, creates inactive callback.
    callback()
    {
    }

    /// \brief      Constructor, creates active callback.
    callback(std::function<void(A ...)> functor, link_element link_element) :
        link_element(std::move(link_element)),
        m_functor(std::move(functor))
    {
    }
    /// \brief      Invokes function `T`, if the callback is still active.
    void invoke(A && ... args) const
    {
        if (link_element::linked())
        {
            m_functor(std::forward<A>(args) ...);
        }
    }

private:
    std::function<void(A ...)> m_functor;
};

/// \brief      Scope-guarded callback to interface.
///
/// Callback is bound to a interface pointer and allows invoking methods
/// from it. Callback requires `callback_guard` to be alive, otherwise it won't
/// be invoked.
template<typename T, template <typename F> typename I>
class callback<T, typename std::enable_if_t<std::is_class_v<T>>> :
    private link_element
{
public:
    /// \brief      Default constructor, creates inactive callback.
    callback() :
        m_interface_ptr(nullptr)
    {
    }

    /// \brief      Constructor, creates active callback.
    callback(T & inteface_ref, link_element link_element) :
        link_element(std::move(link_element)),
        m_interface_ptr(&inteface_ref)
    {
    }

    /// \brief      Invokes method from `T`, if the callback is still active.
    template<typename ... A>
    void
    invoke(void (T:: * method_ptr)(A ...) , A && ... args) const
    {
        if (link_element::linked())
        {
            (m_interface_ptr->*method_ptr)(std::forward<A>(args) ...);
        }
    }

private:
    T * m_interface_ptr;
};
