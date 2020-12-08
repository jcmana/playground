#pragma once

#include <tuple>
#include <type_traits>

template<typename T>
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
