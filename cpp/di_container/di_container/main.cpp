#include <memory>
#include <tuple>

#include "di.hpp"

struct A
{
};

struct B
{
};

int main()
{
    std::tuple<A, di_overload<B, 'm'>, di_overload<B, 's'>> t;
    auto & ref = std::get<di_overload<B, 'm'>>(t);

    const auto dep = di(A(), B(), int(0), int(7));
    const auto t = dep.get<A>();

    return 0;
}