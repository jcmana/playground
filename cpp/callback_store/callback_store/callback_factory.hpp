#pragma once

#include <functional>

#include "callback.hpp"
#include "callback_guard.hpp"

/// \brief      Creates `callback` and its `callback_guard` from `callable`.
template<typename T>
auto make_callback(T callable)
{
    auto [link_a, link_b] = make_link();

    return std::tuple
    {
        callback<T>(std::move(callable), std::move(link_a)),
        callback_guard<T>(std::move(link_b))
    };
}
