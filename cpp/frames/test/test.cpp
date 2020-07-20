#include <iostream>

#include "../frames/frame.hpp"
#include "../frames/xy.hpp"

void main()
{
    constexpr frame<> chamber{{1.0, 0.0}, {0.0, 1.0}, {0.0, 0.0}};
    constexpr frame<> stage{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};
    constexpr frame<> waffer{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};

    const auto waffer_origin = make_xy(0.0, 0.0, waffer);
    const auto chamber_corner = make_xy(10.0, 10.0, chamber);

    const auto x = get_x(waffer_origin, waffer);
    const auto y = get_y(waffer_origin, waffer);

    std::cout << "waffer origin = (" << x << ", " << y << ")" << std::endl;
}
