#pragma once  

namespace Math {

class Functions
{
public:
    Functions(double a);
    ~Functions();

    double Process(double a, double b);

private:
    double m_a;
};

} // namespace Math
