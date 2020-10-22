#include <iostream>
#include <tuple>
#include <string>

struct s
{
    int a;
    int b;
};

void main()
{
    // rref
    {
        auto t = std::tuple(std::string("a"), std::string("b"));

        auto && [a, b] = t;

        // Modifies `t`
        auto x = std::move(a);
        b = "n";

        __asm nop;
    }

    // ref
    {
        s data = {4, 5};

        auto & [a, b] = data;

        // Modifies `data`
        a = 7;
        b = 8;
    }

    // value
    {
        s data = {4, 5};

        auto [a, b] = data;

        // Modifies copies `a` and `b`
        a = 7;
        b = 8;
    }
}