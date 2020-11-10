#pragma once

#include <functional>

#include "callback.hpp"
#include "callback_guard.hpp"

/// \brief      Creates active `callback` and appropriate `callback_guard`.
/// \relates    callback<T>
template<typename F, typename = std::enable_if_t<std::is_invocable_v<F> == true>>
std::tuple<callback<F>, callback_guard<F>> make_callback(std::function<F> functor)
{
    auto [link_a, link_b] = make_link();

    return 
    {
        callback<F>(std::move(functor), std::move(link_a)), 
        callback_guard<F>(std::move(link_b))
    };
}

/// \brief      Creates active `callback` and appropriate `callback_guard`.
/// \param      interface_ref       Reference to callback interface impl.
/// \relates    callback<T>
template<typename T, typename = std::enable_if_t<std::is_invocable_v<T> == false>>
std::tuple<callback<T>, callback_guard<T>> make_callback(T & interface_ref)
{
    auto [link_a, link_b] = make_link();

    return
    {
        callback<T>(interface_ref, std::move(link_a)),
        callback_guard<T>(std::move(link_b))
    };
}
