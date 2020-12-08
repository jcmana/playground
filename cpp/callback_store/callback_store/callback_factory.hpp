#pragma once

#include <functional>

#include "callback.hpp"
#include "callback_guard.hpp"

/*
/// \brief      Creates `callback` from `std::function`.
template<typename ... A>
auto make_callback(std::function<void(A ...)> functor)
{
    auto [link_a, link_b] = make_link();

    return std::tuple
    {
        callback<void(A ...)>(std::move(functor), std::move(link_a)),
        callback_guard<void(A ...)>(std::move(link_b))
    };
}

/// \brief      Creates `callback` from plain function.
template<typename ... A>
auto make_callback(void(functor)(A ...))
{
    auto [link_a, link_b] = make_link();

    return make_callback(std::function(functor));
}

/// \brief      Creates `callback` from `method_ptr` and `interface_ref`.
template<typename T, typename ... A>
auto make_callback(void(T:: * method_ptr)(A ...), T & interface_ref)
{
    auto [link_a, link_b] = make_link();

    return make_callback(std::function<void(A ...)>(std::bind(method_ptr, &interface_ref)));
}

/// \brief      Creates interface `callback` from `interface_ref`.
template<typename T>
auto make_callback(T & interface_ref)
{
    auto [link_a, link_b] = make_link();

    return std::tuple
    {
        callback<T>(interface_ref, std::move(link_a)),
        callback_guard<T>(std::move(link_b))
    };
}
*/

template<typename I>
auto make_callback(I invocation)
{
    auto [link_a, link_b] = make_link();

    return std::tuple
    {
        callback<I>(std::move(invocation), std::move(link_a)),
        callback_guard<I>(std::move(link_b))
    };
}
