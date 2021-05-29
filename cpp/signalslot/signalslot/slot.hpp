#pragma once

#include <functional>

template<typename ... A>
struct slot
{
    template< class Function/*, class ... A*/>
    explicit slot(Function && f/*, A &&... args*/) :
        m_function(std::move(f))
    {
    }

    void receive(A ... arguments)
    {
        m_function(std::forward<A>(arguments) ...);
    }

    std::function<void(A ...)> m_function;
};