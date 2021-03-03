#pragma once

#include "traits.hpp"

namespace invocation
{

template<typename F>
class functor
{
public:
    functor(F f) :
        m_functor(f)
    {
    }

    template<typename ... A>
    auto invoke(A ... args)
    {
        return m_functor(std::forward<A>(args) ...);
    }

private:
    F m_functor;
};

} // namespace invocation
