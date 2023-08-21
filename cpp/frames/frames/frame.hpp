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

    frame() :
        base_x{1.0, 0.0},
        base_y{0.0, 1.0},
        origin{0.0, 0.0}
    {
    }

    frame(basis base_x, basis base_y, basis origin) :
        base_x(base_x),
        base_y(base_y),
        origin(origin)
    {
    }

    /// \returns        Local `xy` of `coordinate` with respect to this `frame`.
    xy<void> localize(const xy<S> & coordinate) const
    {
        return 
        {
            base_x.x * (coordinate.x - origin.x) + base_y.x * (coordinate.y - origin.y),
            base_x.y * (coordinate.x - origin.x) + base_y.y * (coordinate.y - origin.y)
        };
    }

    /// \returns        `xy` of local `coordinate` from `frame` in space `S`.
    xy<S> generalize(const xy<void> & coordinate) const
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

        const auto base_determinant = base_x.x * base_y.y - base_x.y * base_y.x;

        const frame<S>::basis base_x_adjugate = {+base_y.y, -base_x.y};
        const frame<S>::basis base_y_adjugate = {-base_y.x, +base_x.x};

        const auto base_x = coordinate.x / base_determinant;
        const auto base_y = coordinate.y / base_determinant;

        const auto coordinate_x = (base_x_adjugate.x * base_x + base_y_adjugate.x * base_y) + origin.x;
        const auto coordinate_y = (base_x_adjugate.y * base_x + base_y_adjugate.y * base_y) + origin.y;

        return 
        {
            coordinate_x, 
            coordinate_y
        };
    }

    /// \brief      Basis vector defining X-axis.
    basis base_x;
    /// \brief      Basis vector defining Y-axis.
    basis base_y;

    /// \brief      Basis origin.
    basis origin;
};

template<typename S>
constexpr xy<void> operator  &(const xy<S> & coordinate, const frame<S> & source)
{
    return source.localize(coordinate);
}

template<typename S>
constexpr xy<S> operator  &(const xy<void> & coordinate, const frame<S> & target)
{
    return target.generalize(coordinate);
}
