#include <iostream>

#include "../frames/frame.hpp"
#include "../frames/xy.hpp"

template<typename S>
std::ostream & operator <<(std::ostream & stream, xy<S> coordinate)
{
    return stream << "[" << coordinate.x << ", " << coordinate.y << "]";
}

int main()
{
    // Operator change of basis:
    if (false)
    {
        frame<> chamber{{1.0, 0.0}, {0.0, 1.0}, {20.0, 20.0}};
        frame<> stage{{100.0, 0.0}, {0.0, 100.0}, {300.0, -100.0}};
        frame<> waffer{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};
        
        const auto waffer_origin = xy<void>{0.0, 0.0} & waffer;
        const auto waffer_origin_at_chamber = waffer_origin & chamber;
        const auto waffer_origin_at_stage = waffer_origin & stage;

        std::cout << "waffer origin = " << waffer_origin << std::endl;
        std::cout << "waffer origin at chamber = " << waffer_origin_at_chamber << std::endl;
        std::cout << "waffer origin at stage = " << waffer_origin_at_stage << std::endl;
    }

    // Strong-typing:
    if (false)
    {
        struct space_m {};
        struct space_n {};

        frame<space_m> a;
        frame<space_n> b;

        const auto om = xy<void>{0, 0} & a;
        const auto on = xy<void>{0, 0} & b;
        const auto ov = xy<void>{};

        //om = ov;      // error: no operator = matches
        //om = on;      // error: no operator = matches
    }

    // Practical usage:
    if (true)
    {
        struct space_pattern {};
        struct space_DAC {};

        auto size_nm = 100'000;
        auto resolution = 65'536;
        auto step = double(size_nm) / double(resolution);

        frame<space_pattern> patternWF
        {
            {1, 0},
            {0, 1},
            {5, 5},
        };
        frame<space_pattern> patternWFDAC
        {
            {step, 0},
            {0, step},
            {patternWF.origin.x - size_nm, patternWF.origin.y - size_nm}
        };

        auto a = xy<void>{0, 0} & patternWF;
        auto b = a & patternWFDAC;

        std::cout << b << std::endl;
    }
}
