#pragma once

#include <utility>
#include <type_traits>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback_guard;

/// \brief      Scope-guarded callback.
///
/// Callback is mapped to a interface pointer and allows invoking methods
/// from it. Callback requires `callback_guard` to be alive, otherwise it can't
/// be invoked.
///
/// Implementation refuses lambdas or binders, because in this context they are 
/// typical source of issues.
template<typename T>
class callback : private link_element
{
public:
    friend class callback_guard<T>;

public:
    /// \brief      Default constructor, creates inactive callback.
    callback();

    /// \brief      Constructor, creates active callback.
    explicit callback(T & inteface_ref, link_element && link_element_rref);

    /// \brief      Invokes method from `T`, if the callback is still active.
    template<typename F, typename ... Args>
    typename std::enable_if_t<std::is_class<T>::value == true>
    invoke(F method_ptr, Args && ... args) const;

    /// \brief      Invokes function `T`, if the callback is still active.
    template<typename ... Args>
    typename std::enable_if_t<std::is_class<T>::value != true>
    invoke(Args && ... args) const;

    /// \brief      Creates active `callback` and appropriate `callback_guard`.
    template<typename T>
    friend std::tuple<callback<T>, callback_guard<T>> make_callback(T & interface_ref);

private:
    T * m_interface_ptr;
};

#pragma region callback implementation:

template<typename T>
callback<T>::callback() :
    link_element(),
    m_interface_ptr(nullptr)
{
}

template<typename T>
callback<T>::callback(T & inteface_ref, link_element && link_element_rref) :
    link_element(std::move(link_element_rref)),
    m_interface_ptr(&inteface_ref)
{
}

template<typename T>
template<typename F, typename ... Args>
typename std::enable_if_t<std::is_class<T>::value == true>
callback<T>::invoke(F method_ptr, Args && ... args) const
{
    if (link_element::linked())
    {
        (m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
    }
}

template<typename T>
template<typename ... Args>
typename std::enable_if_t<std::is_class<T>::value != true> 
callback<T>::invoke(Args && ... args) const
{
    if (link_element::linked())
    {
        m_interface_ptr(std::forward<Args>(args) ...);
    }
}

template<typename T>
std::tuple<callback<T>, callback_guard<T>> make_callback(T & interface_ref)
{
    auto link = make_link();

    callback<T> c(interface_ref, std::move(std::get<0>(link)));
    callback_guard<T> g(std::move(std::get<1>(link)));

    return std::make_tuple(std::move(c), std::move(g));
}

#pragma endregion
