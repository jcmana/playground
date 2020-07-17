#include "frame.h"

constexpr frame::frame() :
    base_x{1.0, 0.0},
    base_y{0.0, 1.0},
    origin{0.0, 0.0}
{
}

constexpr frame::frame(basis base_x, basis base_y, basis origin) :
    base_x(base_x),
    base_y(base_y),
    origin(origin)
{
}
