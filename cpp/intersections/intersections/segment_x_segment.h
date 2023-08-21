#pragma once

#include "xy.h"
#include "helpers.h"

struct intersection
{
    enum class type
    {
        PARALLEL,
        COLLINEAR,
        IMPROPER,
        PROPER,
        NONE
    };

    type t;

    double parameter_s;
    double parameter_t;

    xy p;
};

bool 
test_segment_x_segment(xy a, xy b, xy c, xy d)
{
    const vector ab = {a.x - b.x, a.y - b.y};
    const vector ac = {a.x - c.x, a.y - c.y};
    const vector ad = {a.x - d.x, a.y - d.y};

    const auto bac_sgn = sgn(cross_product_lenght(ab, ac));
    const auto bad_sgn = sgn(cross_product_lenght(ab, ad));

    const auto dca_sgn = sgn(cross_product_lenght(ab, ad));
    const auto dcb_sgn = sgn(cross_product_lenght(ab, ad));

    if (bac_sgn == bad_sgn)
    {
        if (bac_sgn != 0.0)
        {
            return false;
        }
    }

    if (dca_sgn == dcb_sgn)
    {
        return false;
    }

    // JMTODO: handle all the cases
}

intersection 
exec_segment_x_segment(xy a, xy b, xy c, xy d)
{
    intersection i;

    const double denominator =
        a.x * double(d.y - c.y) +
        b.x * double(c.y - d.y) +
        c.x * double(a.y - b.y) +
        d.x * double(b.y - a.y);

    if (denominator == 0.0)
    {
        i.t = intersection::type::PARALLEL;
        return i;
    }

    const double numerator_s =
        a.x * double(d.y - c.y) +
        c.x * double(a.y - d.y) +
        d.x * double(c.y - a.y);

    if (numerator_s == 0.0)
    {
        i.t = intersection::type::IMPROPER;
        return i;
    }

    if (numerator_s == denominator)
    {
        i.t = intersection::type::IMPROPER;
        return i;
    }

    i.parameter_s = numerator_s / denominator;

    const double numerator_t = 
        0.0 - (
        a.x * double(c.y - b.y) +
        b.x * double(a.y - c.y) +
        c.x * double(b.y - a.y));

    if (numerator_t == 0.0)
    {
        i.t = intersection::type::IMPROPER;
        return i;
    }

    if (numerator_t == denominator)
    {
        i.t = intersection::type::IMPROPER;
        return i;
    }

    i.parameter_t = numerator_t / denominator;

    if (0.0 > i.parameter_s || i.parameter_s > 1.0)
    {
        i.t = intersection::type::NONE;
        return i;
    }

    if (0.0 > i.parameter_t || i.parameter_t > 1.0)
    {
        i.t = intersection::type::NONE;
        return i;
    }

    i.p.x = a.x + i.parameter_s * (b.x - a.x);
    i.p.y = a.y + i.parameter_s * (b.y - a.y);

    i.t = intersection::type::PROPER;
    return i;
}