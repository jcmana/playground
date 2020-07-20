#include "../frames/frame.hpp"
#include "../frames/xy.hpp"

struct frame_pattern
{
};

void main()
{
    frame<frame_default> chamber{{1.0, 0.0}, {0.0, 1.0}, {0.0, 0.0}};
    frame<frame_default> stage{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};
    frame<frame_default> waffer{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};

    xy<frame_default> waffer_origin = waffer(0.0, 0.0);
}
