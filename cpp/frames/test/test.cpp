#include "../frames/frame.h"

namespace frames
{

struct standard
{
};

struct pattern
{
};

} // namespace frames

template<typename F>
class xy
{
public:
    xy() :
        m_x(0.0),
        m_y(0.0)
    {
    }

    xy(double x, double y, const frame<F> & frame = frame<F>) :
        m_x(frame.x_base(x)),
        m_y(0.0)
    {
    }

    /// \brief      X-axis value in given `frame`.
    double x(const frame<F> & frame = frame<F>)
    {
        return frame.x_axis(m_x);
    }

private:
    double m_x;
    double m_y;
};

void main()
{
    //constexpr xy<frames::standard> pa{0.0, 0.0};
    //constexpr xy<frames::pattern> pb{0.0, 0.0};

    constexpr frame<frames::standard> chamber{{1.0, 0.0}, {0.0, 1.0}, {0.0, 0.0}};
    constexpr frame<frames::standard> stage{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};
    constexpr frame<frames::standard> waffer{{100.0, 0.0}, {0.0, 100.0}, {450.0, 80.0}};

    xy<frames::standard> waffer_origin{0.0, 0.0, waffer};

    const auto waffer_origin_x = waffer_origin.x(chamber);
}
