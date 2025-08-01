#pragma once

#include "space_default.hpp"

template<typename Space, typename Frame, typename Scalar>
struct xy_frameful;

template<typename Space, typename Frame, typename Scalar>
struct xy_frameful
{
    Scalar x;
    Scalar y;
};

template
struct xy_frameful<void, void, double>;