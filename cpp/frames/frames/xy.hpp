#pragma once

#include "frame_default.hpp"

/// \brief      Coordinates in frame `F`.
template<typename F = frame_default>
struct xy
{
    double x;
    double y;
};
