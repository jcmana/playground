#pragma once

#include <utility>
#include <type_traits>

#include "traits.hpp"

namespace invocation
{

template<typename F>
class lambda
{
public:
    using return_type = typename member_function_traits<decltype(&F::operator())>::return_type;

public:
    lambda()
    {
    }

    lambda(F f) :
        m_lambda(std::move(f))
    {
    }

    template<typename ... A>
    return_type
    invoke(A ... args)
    {
        static_assert(std::is_same_v<std::tuple<A ...>, member_function_traits<decltype(&F::operator())>::arguments_tuple_type>, "invalid parameters for lambda call");
        m_lambda(std::forward<A>(args) ...);
    }

private:
    F m_lambda;
};

}
