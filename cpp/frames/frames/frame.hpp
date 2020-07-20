#pragma once

#include "xy.hpp"

struct frame_default
{
};

template<typename F = frame_default>
struct frame
{
    struct basis
    {
        double x;
        double y;
    };

    constexpr frame() :
        base_x{1.0, 0.0},
        base_y{0.0, 1.0},
        origin{0.0, 0.0}
    {
    }

    constexpr frame(basis base_x, basis base_y, basis origin) :
        base_x(base_x),
        base_y(base_y),
        origin(origin)
    {
    }

    xy<F> operator ()(double local_x, double local_y)
    {
        //const auto x_base_value = base_x.x * local_x;

        return {0.0, 0.0};
    }

    /// \brief      Basis vector defining X-axis.
    const basis base_x;

    /// \brief      Basis vector defining Y-axis.
    const basis base_y;

    /// \brief      Basis origin.
    const basis origin;
};
