#include <functional>

struct s
{
    void method()
    {
    }
};

void main()
{
    s a;
    auto mf = std::mem_fn(&s::method);
    mf(a);
}