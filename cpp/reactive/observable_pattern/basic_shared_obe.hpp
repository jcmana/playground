#pragma once

#include <functional>

#include "basic_obe.hpp"

template<typename T>
struct basic_shared_obe
{
    template<typename TT>
    using functor_type = std::function<TT>;
    using functor_void_type = functor_type<void()>;

    using observable_type = typename basic_obe<std::remove_cv_t<T>, functor_type>;
    using guard_type = typename observable_type::guard_type;

    using value_type = typename observable_type::value_type;
    using observed_type = typename observable_type::observed_type;

    using mutex_type = typename observable_type::mutex_type;
};