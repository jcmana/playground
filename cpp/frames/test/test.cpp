#include "../frames/frame.h"

struct frame_standard : frame
{
};

struct xy
{
    frame_standard f;
    
    double x;
    double y;
};

void main()
{
    frame f{{2.0, 0.0}, {0.0, 2.0}, {0.0, 0.0}};
}
