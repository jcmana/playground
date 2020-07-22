#pragma once

#include "xy.hpp"
#include "frame_default.hpp"

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

    /// \brief      Basis vector defining X-axis.
    basis base_x;

    /// \brief      Basis vector defining Y-axis.
    basis base_y;

    /// \brief      Basis origin.
    basis origin;
};

/// \brief      Computes `xy` in standard `frame` from local coordinates.
template<typename F>
xy<F> make_xy(double local_x, double local_y, const frame<F> & local_frame)
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

    const auto base_determinant = local_frame.base_x.x * local_frame.base_y.y - local_frame.base_x.y * local_frame.base_y.x;

    const auto base_x_adjugate = frame<F>::basis{+local_frame.base_y.y, -local_frame.base_x.y};
    const auto base_y_adjugate = frame<F>::basis{-local_frame.base_y.x, +local_frame.base_x.x};

    local_x = local_x / base_determinant;
    local_y = local_y / base_determinant;

    const auto coordinate_x = (base_x_adjugate.x * local_x + base_y_adjugate.x * local_y) + local_frame.origin.x;
    const auto coordinate_y = (base_x_adjugate.y * local_x + base_y_adjugate.y * local_y) + local_frame.origin.y;

    return {coordinate_x, coordinate_y};
}

/// \brief      Computes `frame` local `x` coordinate from `xy`.
template<typename F>
double get_x(const xy<F> & coordinate, const frame<F> & frame)
{
    return frame.base_x.x * (coordinate.x - frame.origin.x) + frame.base_y.x * (coordinate.y - frame.origin.y);
}

/// \brief      Computes `frame` local `y` coordinate from `xy`.
template<typename F>
double get_y(const xy<F> & coordinate, const frame<F> & frame)
{
    return frame.base_x.y * (coordinate.x - frame.origin.x) + frame.base_y.y * (coordinate.y - frame.origin.y);
}
