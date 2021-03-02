#pragma once

#include <utility>

#include "traits.hpp"

namespace invocation 
{

template<typename F>
class function
{
public:
    function() :
        m_function(nullptr)
    {
    }

    function(F f) :
        m_function(f)
    {
    }

    template<typename ... A>
    typename function_traits<F>::return_type
    invoke(A ... args)
    {
        static_assert(std::is_same_v<std::tuple<A ...>, function_traits<F>::arguments_tuple_type>, "invalid parameters for function call");
        m_function(std::forward<A>(args) ...);
    }

private:
    F m_function;
};

}