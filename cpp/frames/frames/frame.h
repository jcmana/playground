#pragma once

struct frame
{
    struct basis
    {
        double x;
        double y;
    };

    constexpr frame();
    constexpr frame(basis base_x, basis base_y, basis origin);

    /// \brief      Basis vector defining X-axis.
    const basis base_x;

    /// \brief      Basis vector defining Y-axis.
    const basis base_y;

    /// \brief      Basis origin.
    const basis origin;
};
