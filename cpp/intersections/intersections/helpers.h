#pragma once

#include "xy.h"

double triplet_area(xy a, xy b, xy c)
{
    const double area = 
        (b.x - a.x) * (c.x - a.y) -
        (c.x - a.x) * (b.y - a.y);

    return area;
}

double cross_product_lenght(const vector & first, const vector & second)
{
    return ((-first.y) * (second.x - first.x) + (first.x) * (second.y - first.y));
}

template <typename T> char sgn(T value) 
{
    return (T(0) < value) - (value < T(0));
}