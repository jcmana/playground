#pragma once

#include <tuple>
#include <optional>
#include <type_traits>

template<typename A, typename B>
using bidirectional_map = std::tuple<A, B>[];

void undefined_value_mapping();
void invalid_typename();


template<typename T, typename A, typename B, std::size_t N>
constexpr
std::size_t
find(const T & value, std::tuple<A, B> const (&mapping)[N])
{
    for (std::size_t index = 0; index != N; index++)
    {
        if (std::get<T>(mapping[index]) == value)
        {
            return index;
        }
    }

    return N;
}

template<typename A, typename B, std::size_t N>
constexpr
std::size_t
find(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    for (std::size_t index = 0; index != N; index++)
    {
        if (std::get<B>(mapping[index]) == value)
        {
            return index;
        }
    }

    return N;
}

template<typename A, typename B, std::size_t N>
consteval
const B &
cmap(A value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);
    if (i == N)
    {
        undefined_value_mapping();
    }

    return std::get<B>(mapping[i]);;
}

template<typename A, typename B, std::size_t N>
consteval
const A &
cmap(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        undefined_value_mapping();
    }

    return std::get<A>(mapping[i]);
}

template<typename A, typename B, std::size_t N>
std::optional<A>
map(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        return std::nullopt;
    }

    return std::get<A>(mapping[i]);
}

template<typename A, typename B, std::size_t N>
std::optional<B>
map(const A & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        return std::nullopt;
    }

    return std::get<B>(mapping[i]);
}
