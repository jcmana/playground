#pragma once

#include "space_default.hpp"

/// \brief      Coordinates in space `S`.
template<typename S = space_default>
struct xy
{
    double x;
    double y;
};

/// \brief      Local coordinates in a `frame` in `void` space.
/// 
/// This type is bridge between strong-typed `xy`, which ensures using only
/// coordinates in compatible spaces, and between weak-typed `xy`, which depends
/// on programmer's knowledge what frame and space it belongs to.
template struct xy<void>;
