#pragma once

template<typename F>
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

    basis operator ()(xy value) const
    {
        // JMTODO: impl.
    }

    xy operator ()(basis value) const
    {
        // JMTODO: impl.
    }

    /// \brief      Basis vector defining X-axis.
    const basis base_x;

    /// \brief      Basis vector defining Y-axis.
    const basis base_y;

    /// \brief      Basis origin.
    const basis origin;
};
