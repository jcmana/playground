struct xy
{
    xy() :
        x(0.0),
        y(0.0)
    {
    }

    xy(double x, double y) :
        x(x),
        y(y)
    {
    }

    double x;
    double y;
};

struct xy_system
{
    xy_system() :
        parent_ptr(nullptr)
    {
    }

    xy_system(const xy_system & parent_ref) :
        parent_ptr(&parent_ref)
    {
    }

    xy from_parent_to_this(const xy & coordinate)
    {
        return xy();
    }

    xy from_this_to_parent(const xy & coordinate)
    {
        return xy();
    }

    // Parent coordinate system
    const xy_system * parent_ptr;

    // Parent -> This transformation parameters:
    double rotation = 0.0;
    
    double sx = 1.0;
    double sy = 1.0;

    double tx = 0.0;
    double ty = 0.0;
};

void main()
{
    xy p;
    xy_system stage;
    xy_system specimen(stage);
}
