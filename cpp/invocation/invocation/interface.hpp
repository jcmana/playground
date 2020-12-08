#pragma once

#include "traits.hpp"

namespace invocation
{

template<typename T>
class interface
{
public:
    interface(T & interface_ref) :
        m_interface_ref(interface_ref)
    {
    }

    template<typename F, typename ... A>
    typename member_function_traits<F>::return_type
    invoke(F method_ptr, A ... args)
    {
        static_assert(std::is_same_v<std::tuple<A ...>, member_function_traits<F>::arguments_tuple_type>, "invalid parameters for member function");
        (m_interface_ref.*method_ptr)(std::forward<A>(args) ...);
    }

private:
    T & m_interface_ref;
};

}
