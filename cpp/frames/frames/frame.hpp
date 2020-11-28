#pragma once

#include "xy.hpp"
#include "space_default.hpp"

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

/// \brief      Computes `xy` in standard `frame` from local coordinates.
template<typename S>
xy<S> make_xy(double local_x, double local_y, const frame<S> & local_frame)
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

    const frame<S>::basis base_x_adjugate = {+local_frame.base_y.y, -local_frame.base_x.y};
    const frame<S>::basis base_y_adjugate = {-local_frame.base_y.x, +local_frame.base_x.x};

    local_x = local_x / base_determinant;
    local_y = local_y / base_determinant;

    const auto coordinate_x = (base_x_adjugate.x * local_x + base_y_adjugate.x * local_y) + local_frame.origin.x;
    const auto coordinate_y = (base_x_adjugate.y * local_x + base_y_adjugate.y * local_y) + local_frame.origin.y;

    return {coordinate_x, coordinate_y};
}

/// \brief      Computes `frame` local `x` coordinate from `xy`.
template<typename S>
double get_x(const xy<S> & coordinate, const frame<S> & frame)
{
    return frame.base_x.x * (coordinate.x - frame.origin.x) + frame.base_y.x * (coordinate.y - frame.origin.y);
}

/// \brief      Computes `frame` local `y` coordinate from `xy`.
template<typename S>
double get_y(const xy<S> & coordinate, const frame<S> & frame)
{
    return frame.base_x.y * (coordinate.x - frame.origin.x) + frame.base_y.y * (coordinate.y - frame.origin.y);
}

/// \brief      Returns local `xy` of `coordinate` in `frame`.
template<typename S>
constexpr xy<void> operator  &(xy<S> coordinate, frame<S> frame)
{
    return xy<void>{get_x(coordinate, frame), get_y(coordinate, frame)};
}

/// \brief      Returns `xy` of local `coordinate` from `frame` in space `S`.
template<typename S>
constexpr xy<S> operator  &(xy<void> coordinate, frame<S> frame)
{
    return make_xy(coordinate.x, coordinate.y, frame);;
}
