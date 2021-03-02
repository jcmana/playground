#pragma once

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
    typename member_function_traits<decltype(&F::operator())>::return_type
    invoke(A ... args)
    {
        m_functor(std::forward<A>(args) ...);
    }

private:
    F m_functor;
};


} // namespace invocation
