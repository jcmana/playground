#include <iostream>

#include "../frames/frame.hpp"
#include "../frames/xy.hpp"

struct space_pattern;
struct space_DAC;

template<typename Space, typename Scalar>
std::ostream & operator <<(std::ostream & stream, xy<Space, Scalar> coordinate)
{
    return stream << "[" << coordinate.x << ", " << coordinate.y << "]";
}

xy<space_DAC, double> transform(const xy<space_pattern, double> & source)
{
    return {};
}

template<>
xy<space_DAC, double> transform_implicit(const xy<space_pattern, double> & source)
{
    return {};
}

int main()
{
    // Operator change of basis:
    if (false)
    {
        frame<> chamber{{1.0, 0.0}, {0.0, 1.0}, {20.0, 20.0}};
        frame<> stage{{100.0, 0.0}, {0.0, 100.0}, {300.0, -100.0}};
        frame<> waffer{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};
        
        const auto waffer_origin = xy<void, double>{0.0, 0.0} & waffer;
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

        const auto om = xy<void, double>{0, 0} & a;
        const auto on = xy<void, double>{0, 0} & b;
        const auto ov = xy<void, double>{0, 0};

        //om = ov;      // error: no operator = matches
        //om = on;      // error: no operator = matches
    }

    // Practical usage:
    if (false)
    {
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

        auto a = xy<void, double>{0, 0} & patternWF;
        auto b = a & patternWFDAC;

        std::cout << b << std::endl;
    }

    // Different types in different spaces:
    // JMTODO:
    if (false)
    {
        xy<space_pattern, double> coordinatePattern = {0, 0};
        xy<space_DAC, std::int64_t> coordinateDAC = {0, 0};
    }

    // Strongly-typed concrete frames:
    if (true)
    {
        // From above test, pattern and DAC are both the same space, the coordinates can be converted
        // between them, so having different space doesn't make sense. 
        //
        // What we need is having the same space but strongly-typed frames to enforce conversion but keep
        // them compatible.
        //
        // Or maybe not, maybe the name space is just confusing and that's why I'm trying to add frame 
        // as well.

        xy<space_pattern, double> coordinatePattern = {0, 0};
        xy<space_DAC, double> coordinateDAC = {0, 0};

        // JMTODO: does it make sense or is space enough?
        // JMTODO: how to convert between them?

        auto function = [](xy<space_DAC, double> coordinate)
        {
            std::cout << coordinate << std::endl;
        };

        // Conversion definition at runtime:
        {
            auto patternToDAC = [](xy<space_pattern, double> coordinatePattern)
            {
                xy<space_DAC, double> coordinateDAC = {0, 0};
                return coordinateDAC;
            };

            function(patternToDAC(coordinatePattern));
        }

        // Conversion definition at compile-time:
        {
            // Explicit
            function(transform(coordinatePattern));

            // Implicit
            coordinateDAC = coordinatePattern;
            coordinateDAC = transform_implicit<space_pattern, double, space_DAC, double>(coordinatePattern);
            
            function(coordinatePattern);
        }
    }

    // `frame` impl. should be just one the ways to move between different but compatible frames of 
    // reference. User has to be able to implement their own methods to do that in situations like:
    // - having different base `Scalar` types
    // - having a scale or skew relation (affine) between compatible frames
    // - non-linear frames
}
