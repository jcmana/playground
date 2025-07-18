#pragma once

#include <array>
#include <tuple>
#include <span>

static constexpr std::tuple<long, double> MAP[] =
{
    {7, 0.7},
    {6, 0.06},
};

constexpr auto translate(auto input, auto map)
{
    /*
    for (const auto & entry : map)
    {
        const auto [lhs, rhs] = entry;
        
        if (lhs == input)
        {
            return rhs;
        }
    }
    */
}

template<double>
struct Map;

template<>
struct Map<0.7>
{
    static constexpr long value = 7;

};