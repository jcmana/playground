#include <iostream>

#include "xy.h"
#include "segment.h"

#include "segment_x_segment.h"

std::ostream & operator <<(std::ostream & os, const intersection::type & t)
{
    switch (t)
    {
        case intersection::type::PARALLEL:
            return (os << "PARALLEL");
        case intersection::type::IMPROPER:
            return (os << "IMPROPER");
        case intersection::type::NONE:
            return (os << "NONE");
        case intersection::type::PROPER:
            return (os << "PROPER");
    }

    return os;
}

std::ostream & operator <<(std::ostream & os, const intersection & i)
{
    switch (i.t)
    {
        case intersection::type::PARALLEL:
        case intersection::type::NONE:
            return (os << i.t);
        case intersection::type::IMPROPER:
        case intersection::type::PROPER:
            return (os << i.t << "[" << i.p.x << ", " << i.p.y << "]");
    }

    return os;
}

void main()
{
    // Orthogonal:
    if (false)
    {
        segment l = {xy{-1, 0}, xy{+1, 0}};

        segment a = {xy{0, +1}, xy{0, -1}};
        segment b = {xy{0, +1}, xy{0, -1'000}};
        segment c = {xy{0, +1}, xy{0, -1'000'000}};
        segment d = {xy{0, +1}, xy{0, -1'000'000'000}};

        const auto ia = exec_segment_x_segment(l.a, l.b, a.a, a.b);
        const auto ib = exec_segment_x_segment(l.a, l.b, b.a, b.b);
        const auto ic = exec_segment_x_segment(l.a, l.b, c.a, c.b);
        const auto id = exec_segment_x_segment(l.a, l.b, d.a, d.b);

        std::cout << ia << "\n";
        std::cout << ib << "\n";
        std::cout << ic << "\n";
        std::cout << id << "\n";
    }

    // Skewed:
    if (false)
    {
        segment l = {xy{-1'000'000, 0}, xy{+1'000'000, 0}};

        segment a = {xy{+1'000'001, +1'000'000}, xy{-999'999, -1'000'000}};
        segment b = {xy{+1'000'000, +1}, xy{-1'000'000, -1}};
        segment c = {xy{+1'000'000'000, +1}, xy{-1'000'000'000, -1}};
        segment d = {xy{+1e16, +1}, xy{1, 0}};

        const auto ia = exec_segment_x_segment(l.a, l.b, a.a, a.b);
        const auto ib = exec_segment_x_segment(l.a, l.b, b.a, b.b);
        const auto ic = exec_segment_x_segment(l.a, l.b, c.a, c.b);
        const auto id = exec_segment_x_segment(l.a, l.b, d.a, d.b);

        std::cout << ia << "\n";
        std::cout << ib << "\n";
        std::cout << ic << "\n";
        std::cout << id << "\n";
    }

    // Parallel:
    if (false)
    {
        segment l = {xy{-1'000'000, 0}, xy{+1'000'000, 0}};

        segment a = {xy{-1'000'000, 1}, xy{+1'000'000, 1}};
        segment b = {xy{-2'000'000, 0}, xy{+2'000'000, 0}};

        const auto ia = exec_segment_x_segment(l.a, l.b, a.a, a.b);
        const auto ib = exec_segment_x_segment(l.a, l.b, b.a, b.b);

        std::cout << ia << "\n";
        std::cout << ib << "\n";
    }

    // Comparison:
    if (false)
    {
        segment l = {xy{30289, 66797}, xy{30290, 66797}};

        segment a = {xy{30458, 66797}, xy{30445, 66748}};
        segment b = {xy{30469, 66812}, xy{30458, 66797}};

        const auto ia = exec_segment_x_segment(l.a, l.b, a.a, a.b);
        const auto ib = exec_segment_x_segment(l.a, l.b, b.a, b.b);

        std::cout << ia << "\n";
        std::cout << ib << "\n";
    }

    // Test:
    if (true)
    {
        segment l = {xy{-1, 0}, xy{+1, 0}};
        segment m = {xy{-2, -2}, xy{+2, +2}};

        segment a = {xy{0, +1}, xy{0, -1}};
        segment b = {xy{-1, 1}, xy{+1, 4}};
        segment c = {xy{0, +1}, xy{0, 0}};
        segment d = {xy{-2, +2}, xy{+1, -2}};

        const auto ta = test_segment_x_segment(l.a, l.b, a.a, a.b);
        const auto tb = test_segment_x_segment(l.a, l.b, b.a, b.b);
        const auto tc = test_segment_x_segment(l.a, l.b, c.a, c.b);
        const auto tmd = test_segment_x_segment(m.a, m.b, d.a, d.b);

        std::cout << ta << "\n";
        std::cout << tb << "\n";
        std::cout << tc << "\n";
        std::cout << tmd << "\n";
    }
}
