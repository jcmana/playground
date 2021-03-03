#pragma once

#include <type_traits>
#include <tuple>

template<typename T>
struct function_traits
{
};

template<typename R, typename ... A>
struct function_traits<R(A ...)>
{
    using return_type = R;
    using arguments_tuple_type = std::tuple<A ...>;
};

template<typename R, typename ... A>
struct function_traits<R(*)(A ...)>
{
    using return_type = R;
    using arguments_tuple_type = std::tuple<A ...>;
};

template<typename F, typename ... A>
struct member_function_traits
{
};

template<typename T, typename R, typename ... A>
struct member_function_traits<R(T:: *)(A ...)>
{
    using return_type = R;
    using arguments_tuple_type = std::tuple<A ...>;
};

template<typename T, typename R, typename ... A>
struct member_function_traits<R(T:: *)(A ...) const>
{
    using return_type = R;
    using arguments_tuple_type = std::tuple<A ...>;
};
