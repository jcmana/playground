#pragma once

#include "traits.hpp"

namespace invocation
{

template<typename T>
class interface
{
public:
    interface() :
        m_interface_ptr(nullptr)
    {
    }

    interface(T & interface_ref) :
        m_interface_ptr(&interface_ref)
    {
    }

    template<typename F, typename ... A>
    auto invoke(F method_ptr, A ... args)
    {
        static_assert(std::is_same_v<std::tuple<A ...>, member_function_traits<F>::arguments_tuple_type>, "invalid parameters for member function call");
        return (*m_interface_ptr.*method_ptr)(std::forward<A>(args) ...);
    }

private:
    T * m_interface_ptr;
};

}
