#pragma once

#include "xy.hpp"
#include "space_default.hpp"

/// \brief      Defines frame of reference in a space `S`.
template<typename S = space_default>
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

    /// \brief      Basis vector defining X-axis.
    basis base_x;

    /// \brief      Basis vector defining Y-axis.
    basis base_y;

    /// \brief      Basis origin.
    basis origin;
};

/// \brief      Returns local `xy` of `coordinate` in `source`.
template<typename S>
constexpr xy<void> operator  &(xy<S> coordinate, frame<S> source)
{
    return 
    {
        source.base_x.x * (coordinate.x - source.origin.x) + source.base_y.x * (coordinate.y - source.origin.y),
        source.base_x.y * (coordinate.x - source.origin.x) + source.base_y.y * (coordinate.y - source.origin.y)
    };
}

/// \brief      Returns `xy` of local `coordinate` from `frame` in space `S`.
template<typename S>
constexpr xy<S> operator  &(xy<void> coordinate, frame<S> target)
{
    // Matrix inversion:
    //
    //       1
    // A = ----- adjugate(A)
    //     | A |
    //
    //         ( a  b )   ( +d -b )
    // adjugate(      ) = (       )
    //         ( c  d )   ( -c +d )

    const auto base_determinant = target.base_x.x * target.base_y.y - target.base_x.y * target.base_y.x;

    const frame<S>::basis base_x_adjugate = {+target.base_y.y, -target.base_x.y};
    const frame<S>::basis base_y_adjugate = {-target.base_y.x, +target.base_x.x};

    const auto base_x = coordinate.x / base_determinant;
    const auto base_y = coordinate.y / base_determinant;

    const auto coordinate_x = (base_x_adjugate.x * base_x + base_y_adjugate.x * base_y) + target.origin.x;
    const auto coordinate_y = (base_x_adjugate.y * base_x + base_y_adjugate.y * base_y) + target.origin.y;

    return 
    {
        coordinate_x, 
        coordinate_y
    };
}
