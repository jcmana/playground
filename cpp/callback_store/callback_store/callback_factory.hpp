#pragma once

#include <functional>

#include "callback.hpp"
#include "callback_guard.hpp"

/// \brief      
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
